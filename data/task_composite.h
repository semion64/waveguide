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
	double_long disloc_number;
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

wg::PhotonStructure EmptyPhotonStructure() {
	return wg::PhotonStructure::Create(
		wg::Layer {wg::materials::CreateWithParams(1), 0}, // air
		wg::Layer {_param.material_even, _param.width_even},// polycore
		wg::Layer {_param.material_odd, _param.width_odd}, // fp
			_param.count_layers);
} 

wg::PhotonStructure DislocPhotonStructure(double_long eps_real, int layer_number = _param.disloc_number, double_long layer_width = _param.disloc_width, double_long eps_imag = 0, double_long mu_real = 1, double_long mu_imag = 0) {
	auto ps = EmptyPhotonStructure();
	ps.ReplaceLayer(wg::Layer {wg::materials::CreateWithParams(eps_real, eps_imag, mu_real, mu_imag), layer_width}, layer_number);
	return ps;
} 

Offset CalibOn(const std::string& exp_file, double_long calib_eps, double_long calib_width = _param.disloc_width,  bool show_graphics = false) {
	wg::PhotonStructure theor_ps = DislocPhotonStructure(calib_eps, _param.disloc_number, calib_width);
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

double_long EpoxEpsCalc(const std::string& exp_file, Offset offset, bool show_graphics = false) {
	wg::f_vector_load fv = agilent::LoadF(exp_file);
	wg::PointR min_exp = wg::calc::FindMinR(agilent::LoadSpectrR(exp_file));
	auto ps = EmptyPhotonStructure();
	auto solution = stat_analize::BackTask(2.0, 3.0, 100, 0.01,  
		[&ps, &min_exp, &fv, &offset](double_long e) {
             return std::sqrt(
				std::pow(
					wg::calc::FindMinR(
						fv, ps.ReplaceLayer(
							wg::Layer {wg::materials::CreateWithParams(e), _param.disloc_width}, _param.disloc_number
						)
					).f + offset.dx - min_exp.f, 2
				)
			) / 2;
        });
        
	wg::PhotonStructure theor_ps = DislocPhotonStructure(solution.first);
	DataXY exp_spectr = agilent::LoadSpectrR(exp_file);
	if(show_graphics) {
		auto plot = wg::SpectrDrawer("Epox");
		plot.Add("epox_{experiment}", exp_file);
		plot.Add("epox_{theor}", theor_ps, fv, offset);
		plot.Draw();
	}
	
	return solution.first;
}

void Analize() {
	Offset offset = CalibOn("data/fp_3.95.s2p", 2.0, _param.disloc_width, true);
	
	std::cout << "[calc] offset: " << offset.dx << std::endl;
	double_long eps_epox = EpoxEpsCalc("data/c0_1.s2p", offset, true);
	std::cout << "[calc] eps_epox: " << eps_epox << std::endl;
	
}
}
