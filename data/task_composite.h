#pragma once
#include <memory>
#include "../waveguide.h"
#include "../spectr_drawer.h"
#include "../sample.h"
#include "../calibrator.h"
#include "../permutation.h"
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

wg::CalibratorSpectrR CalibrateOnFp(const std::string& file = "data/fp_3.95.s2p", bool show_graph = false) {
	auto st_fp = DislocStruct(wg::materials::Epsilon {2.0, 0});
	wg::CalibratorSpectrR calibrator = wg::CalibratorSpectrR(*file_ex);
	calibrator.CalibF(st_fp, file, show_graph);
	calibrator.CalibR(st_fp, file, show_graph);
	
	if(show_graph) {
		auto plot = wg::SpectrDrawer("Adjust result: ", *file_ex);
			plot.Add("before", wg::calc::BuildSpectrR(9'400'000'000, 9'800'000'000, 1000000, st_fp));
			calibrator.Adjust(st_fp);
			plot.Add("after", wg::calc::BuildSpectrR(9'400'000'000, 9'800'000'000, 1000000, st_fp));
			plot.Add("experiment", "data/fp_3.95.s2p");
		plot.Draw();
	}
	return calibrator;
}
void Analize() {
	//wg::SampleSet test_set = wg::SampleSet("data/test.set", "s2p", file_ex);
	//test_set.DrawSpectrR("test", {"c10_1", "c10_2"});
	//auto calibrator = CalibrateOnFp("data/fp_3.95.s2p", true);
	//Graph_DependenceFromCNT();
	
	//algebra::vector::Vector<int> a {5, 3, -4}, b {6, 7, -8};
	//std::cout << (a ^ b) << std::endl; 
}
}
