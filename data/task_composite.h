#pragma once
#include <memory>
#include "../waveguide.h"
#include "../spectr_drawer.h"
#include "../sample.h"
#include "../calibrator.h"

namespace task_composite{
	
wg::FileExcluder* file_ex = new wg::FileExcluderAgilent();

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

template <typename Val, typename StructParamFunc>
stat_analize::BackTaskResult<Val>  BackTaskByPeackFreq(const std::string& file, Val val_start, Val val_end, int N, Val delta, StructParamFunc ps_param_func, Offset offset) {
	 return wg::calc::BackTaskByPeackFreq(file_ex->LoadSpectrR(file), file_ex->LoadF(file), val_start, val_end, N, delta, ps_param_func, offset);
}

wg::Struct EmptyStruct() {
	return wg::Struct::Create(
		wg::Layer {wg::materials::CreateWithParams(1), 0}, // air
		wg::Layer {_param.material_even, _param.width_even},// polycore
		wg::Layer {_param.material_odd, _param.width_odd}, // fp
			_param.count_layers);
} 

wg::Struct DislocStruct(wg::materials::Epsilon eps, double_long disloc_width = _param.disloc_width, int disloc_pos = _param.disloc_pos, wg::materials::Mu mu = wg::materials::Mu {1, 0}) {
	auto ps = EmptyStruct();
	ps.ReplaceLayer(wg::Layer {wg::materials::Material{eps, mu}, disloc_width}, disloc_pos);
	return ps;
}


Offset CompareFiles(const std::vector<std::string>& exp_files) {
		auto plot = wg::SpectrDrawer("Compare", *file_ex);
		for(const auto& file : exp_files) {
			plot.Add(file, file);
		}
		plot.Draw();
}
/*
double_long EpoxEpsRealCalc(const std::string& exp_file, Offset offset, bool show_graphics = false) {
	wg::f_vector_load fv = file_ex->LoadF(exp_file);
	wg::PointR min_exp = wg::calc::FindMinR(file_ex->LoadSpectrR(exp_file));
	auto ps = EmptyStruct();
    const auto [solution_eps, s, arr] = wg::calc::BackTaskByPeackFreq(file_ex->LoadSpectrR(exp_file), fv, 2.0, 3.0, 100, 0.01, 
			[&ps](double_long val) {
				return ps.ReplaceLayer(wg::Layer {wg::materials::CreateWithParams(val), _param.disloc_width}, _param.disloc_pos);
			}, offset);
    
	wg::Struct theor_ps = DislocStruct(wg::materials::Epsilon {solution_eps, 0});
	DataXY exp_spectr = file_ex->LoadSpectrR(exp_file);
	if(show_graphics) {
		auto plot = wg::SpectrDrawer("Epox", *file_ex);
		plot.Add("epox_{experiment}", exp_file);
		plot.Add("epox_{theor}", theor_ps, fv);
		plot.Draw();
	}
	
	return solution_eps;
}

double_long EpoxEpsImagCalc(const std::string& exp_file, double_long eps_real, Offset offset, bool show_graphics = false) {
	auto ps = EmptyStruct();
	const auto [solution_eps, s, arr] = BackTaskByPeackFreq(exp_file, 0.0, 1.0, 100, 0.01, 
			[&ps, &eps_real](double_long val) {
				return ps.ReplaceLayer(wg::Layer {wg::materials::CreateWithParams(eps_real, val), _param.disloc_width}, _param.disloc_pos);
			}, offset);

	wg::Struct theor_ps = DislocStruct(wg::materials::Epsilon {eps_real, solution_eps});
	DataXY exp_spectr = file_ex->LoadSpectrR(exp_file);
	wg::f_vector_load fv = file_ex->LoadF(exp_file);
	if(show_graphics) {
		auto plot = wg::SpectrDrawer("Epox_Image", *file_ex);
		plot.Add("epox_{experiment}", exp_file);
		plot.Add("epox_{theor}", DislocStruct(wg::materials::Epsilon {eps_real, solution_eps}), file_ex->LoadF(exp_file));
		plot.Draw();
	}
	
	return solution_eps;
}
*/
void Graph_DependenceFromCNT() {
	wg::SampleSet test_set = wg::SampleSet("data/test.set", "s2p", *file_ex);
	auto plot = wg::SpectrDrawer("Dependence from CNT", *file_ex);
	plot.Add(
		"In field", 
		test_set.ParametrDependence("m_cnt_in_comp", [](auto sample) {
				DataXY dt = {{0,0}, {0,0}};
				return wg::calc::FindMinR(sample->GetSpectrR()).R;
			}, {"c10_2", "c11_2", "c12_2", "c13_2", "c14_2"}
		)
	);
	
	plot.Add(
		"Without field", 
		test_set.ParametrDependence("m_cnt_in_comp", [](const auto& sample) {
				return wg::calc::FindMinR(sample->GetSpectrR()).R;
			}, {"c10_1", "c11_1", "c12_1", "c13_1", "c14_1"}
		)
	);	
	
	plot.Draw();
	
	DataXY in_field = test_set.ParametrDependence("m_cnt_in_comp", [](const auto& sample) {
				return wg::calc::FindMinR(sample->GetSpectrR()).R;
			}, 
			{"c10_2", "c11_2", "c12_2", "c13_2", "c14_2"}
		);
	DataXY without_field = test_set.ParametrDependence("m_cnt_in_comp", [](const auto& sample) {
				return wg::calc::FindMinR(sample->GetSpectrR()).R;
			}, 
			{"c10_1", "c11_1", "c12_1", "c13_1", "c14_1"}
		);
	auto plot_dr = wg::SpectrDrawer("Field-Without field from CNT", *file_ex);
	plot_dr.Add("Without field", in_field - without_field);
	
	plot_dr.Draw();
}

void Analize() {
	//Offset offset = CalibOnXOffset("data/fp_3.95.s2p", wg::materials::Epsilon{2.0, 0.0}, _param.disloc_width, true);
	//Offset offset2 = CalibOnXOffset("data/fp_3.95.s2p", wg::materials::Epsilon{2.0, 0.0}, _param.disloc_width, true);
	
	//std::cout << "[calc] offset: " << offset.dx << std::endl;
	//double_long eps_real_epox = EpoxEpsRealCalc("data/c0_2.s2p", offset, true);
	//std::cout << "[calc] eps_real_epox: " << eps_real_epox << std::endl;
	//double_long eps_imag_epox = EpoxEpsImagCalc("data/c0_2.s2p", eps_real_epox, offset, true);
	//std::cout << "[calc] eps_image_epox: " << eps_imag_epox << std::endl;
	
	//wg::SampleSet test_set = wg::SampleSet("data/test.set", "s2p", file_ex);
	//test_set.DrawSpectrR("test", {"c10_1", "c10_2"});
	//Graph_DependenceFromCNT();
	
	auto st_fp = DislocStruct(wg::materials::Epsilon {3.0, 0});
	wg::CalibratorSpectrR calibrator = wg::CalibratorSpectrR(*file_ex);
	calibrator.Calib(st_fp, "data/c10_1.s2p", true);
	
	auto plot = wg::SpectrDrawer("Ftoroplast", *file_ex);
	plot.Add("before adjust", wg::calc::BuildSpectrR(9'200'000'000, 9'500'000'000, 1000000, st_fp));
	calibrator.Adjust(st_fp);
	plot.Add("after adjust", wg::calc::BuildSpectrR(9'200'000'000, 9'500'000'000, 1000000, st_fp));
	plot.Draw();
	
	/*wg::Struct st = DislocStruct(wg::materials::Epsilon {3.0, 0});
	st.AddLayer(wg::Layer{
		wg::materials::Material {
			wg::materials::Epsilon{1, 0.01},
			wg::materials::Mu{1,0}
		}, 0.01
	}, 1);
	
	auto plot = wg::SpectrDrawer("Epox_Image", *file_ex);
	
	plot.Add("before adjust", wg::calc::BuildSpectrR(9'200'000'000, 10'500'000'000, 1'000'000, DislocStruct(wg::materials::Epsilon {3.0, 0})));
	plot.Add("after adjust", wg::calc::BuildSpectrR(9'200'000'000, 10'500'000'000, 1'000'000, st));
	plot.Draw();*/
	
	
	
}
}
