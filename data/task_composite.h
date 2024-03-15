#pragma once

#include "../waveguide.h"
#include "../spectr_drawer.h"

namespace task_composite{

struct CrystParams {
	int count_layers;
	wg::materials::Material material_even;
	double_long width_even;
	wg::materials::Material material_odd;
	double_long width_odd;
	double_long disloc_pos;
	double_long disloc_width;
	
};

const CrystParams _param {
	11,
	wg::materials::CreateWithParams(9.6), // polycore
	0.001,
	wg::materials::CreateWithParams(2.0), // fp
	0.009,
	6,
	0.00395	
};

template <typename Val, typename PhotonStructureParamFunc>
stat_analize::BackTaskResult<Val>  BackTaskByPeackFreq(const std::string& file, Val val_start, Val val_end, int N, Val delta, PhotonStructureParamFunc ps_param_func, Offset offset) {
	 return wg::calc::BackTaskByPeackFreq(agilent::LoadSpectrR(file), agilent::LoadF(file), val_start, val_end, N, delta, ps_param_func, offset);
}

wg::PhotonStructure EmptyPhotonStructure() {
	return wg::PhotonStructure::Create(
		wg::Layer {wg::materials::CreateWithParams(1), 0}, // air
		wg::Layer {_param.material_even, _param.width_even},// polycore
		wg::Layer {_param.material_odd, _param.width_odd}, // fp
			_param.count_layers);
} 

wg::PhotonStructure DislocPhotonStructure(wg::materials::Epsilon eps, double_long disloc_width = _param.disloc_width, int disloc_pos = _param.disloc_pos, wg::materials::Mu mu = wg::materials::Mu {1, 0}) {
	auto ps = EmptyPhotonStructure();
	ps.ReplaceLayer(wg::Layer {wg::materials::Material{eps, mu}, disloc_width}, disloc_pos);
	return ps;
}

Offset CalibOnXOffset(const std::string& exp_file, wg::materials::Epsilon calib_eps, double_long calib_width = _param.disloc_width,  bool show_graphics = false) {
	wg::PhotonStructure theor_ps = DislocPhotonStructure(calib_eps, calib_width);
	wg::f_vector_load fv = agilent::LoadF(exp_file);
	
	DataXY exp_spectr = agilent::LoadSpectrR(exp_file);
	//DataXY theor_spectr = wg::calc::BuildSpectrR(fv, theor_ps);
	if(show_graphics) {
		auto plot = wg::SpectrDrawer("Ftoroplast");
		plot.Add("fp_{experiment}", exp_file);
		plot.Add("fp_{theor}", theor_ps, fv);
		plot.Draw();
	}
	
	wg::PointR pt_theor = wg::calc::FindMinR(fv, theor_ps);
	wg::PointR pt_exp = wg::calc::FindMinR(exp_spectr);
	
	Offset offset {pt_exp.f - pt_theor.f, 0};
	if(show_graphics) {
		auto plot_offset = wg::SpectrDrawer("Ftoroplast + Offset");
		plot_offset.Add("fp_{experiment}", exp_file);
		plot_offset.Add("fp_{theor} + offset", theor_ps, fv, offset);
		plot_offset.Draw();
	}
	
	return offset;
}
Offset CompareFiles(const std::vector<std::string>& exp_files) {
		auto plot = wg::SpectrDrawer("Compare");
		for(const auto& file : exp_files) {
			plot.Add(file, file);
		}
		plot.Draw();
}

double_long EpoxEpsRealCalc(const std::string& exp_file, Offset offset, bool show_graphics = false) {
	wg::f_vector_load fv = agilent::LoadF(exp_file);
	wg::PointR min_exp = wg::calc::FindMinR(agilent::LoadSpectrR(exp_file));
	auto ps = EmptyPhotonStructure();
    const auto [solution_eps, s, arr] = wg::calc::BackTaskByPeackFreq(agilent::LoadSpectrR(exp_file), fv, 2.0, 3.0, 100, 0.01, 
			[&ps](double_long val) {
				return ps.ReplaceLayer(wg::Layer {wg::materials::CreateWithParams(val), _param.disloc_width}, _param.disloc_pos);
			}, offset);
    
	wg::PhotonStructure theor_ps = DislocPhotonStructure(wg::materials::Epsilon {solution_eps, 0});
	DataXY exp_spectr = agilent::LoadSpectrR(exp_file);
	if(show_graphics) {
		auto plot = wg::SpectrDrawer("Epox");
		plot.Add("epox_{experiment}", exp_file);
		plot.Add("epox_{theor}", theor_ps, fv, offset);
		plot.Draw();
	}
	
	return solution_eps;
}

double_long EpoxEpsImagCalc(const std::string& exp_file, double_long eps_real, Offset offset, bool show_graphics = false) {
	auto ps = EmptyPhotonStructure();
	const auto [solution_eps, s, arr] = BackTaskByPeackFreq(exp_file, 0.0, 1.0, 100, 0.01, 
			[&ps, &eps_real](double_long val) {
				return ps.ReplaceLayer(wg::Layer {wg::materials::CreateWithParams(eps_real, val), _param.disloc_width}, _param.disloc_pos);
			}, offset);

	wg::PhotonStructure theor_ps = DislocPhotonStructure(wg::materials::Epsilon {eps_real, solution_eps});
	DataXY exp_spectr = agilent::LoadSpectrR(exp_file);
	wg::f_vector_load fv = agilent::LoadF(exp_file);
	if(show_graphics) {
		auto plot = wg::SpectrDrawer("Epox_Image");
		plot.Add("epox_{experiment}", exp_file);
		plot.Add("epox_{theor}", DislocPhotonStructure(wg::materials::Epsilon {eps_real, solution_eps}), agilent::LoadF(exp_file), offset);
		plot.Draw();
	}
	
	return solution_eps;
}

void Analize() {
	Offset offset = CalibOnXOffset("data/fp_3.95.s2p", wg::materials::Epsilon{2.0, 0.0}, _param.disloc_width, true);
	std::cout << "[calc] offset: " << offset.dx << std::endl;
	double_long eps_real_epox = EpoxEpsRealCalc("data/c0_2.s2p", offset, true);
	std::cout << "[calc] eps_real_epox: " << eps_real_epox << std::endl;
	double_long eps_imag_epox = EpoxEpsImagCalc("data/c0_2.s2p", eps_real_epox, offset, true);
	std::cout << "[calc] eps_image_epox: " << eps_imag_epox << std::endl;
	
}
}
