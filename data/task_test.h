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


void DrawFiles(std::string title, const std::vector<std::string>& exp_files) {
		auto plot = wg::SpectrDrawer(title, *file_ex);
		for(const auto& file : exp_files) {
			plot.Add(file, file);
		}
		plot.Draw();
}

std::string exp_dir = "data/measure/";
void Graph_DependenceFromOrientFF(bool draw_graph = false) {
	wg::SampleSet set = wg::SampleSet(exp_dir + "orient.set", "s2p", *file_ex);
	if(draw_graph) {
		set.DrawSpectrR("OrientFF 90 degree", {"d2_1", "d2_2"});
		set.DrawSpectrR("OrientFF 0 degree", {"d3_1", "d3_2"});
		set.DrawSpectrR("OrientFF * degree", {"d1_1", "d1_2"});
		set.DrawSpectrR("OrientFF without FF", {"d0_1", "d0_2"});
	}
	
	std::cout << "============Orientation_FF===========" << std::endl;
	std::cout << "Orientff without ff D0_1: " << set.FindMinR("d0_1").R << "\t" << set.FindMinR("d0_1").f << std::endl;
	std::cout << "Orientff without ff D0_2: " << set.FindMinR("d0_2").R << "\t" << set.FindMinR("d0_2").f << std::endl;
	std::cout << "dr: " << set.FindMinR("d0_2").R - set.FindMinR("d0_1").R << "\t" << set.FindMinR("d0_2").f - set.FindMinR("d0_1").f << std::endl << "----------" << std::endl;
	
	std::cout << "Orientff 0 D3_1: " << set.FindMinR("d3_1").R << "\t" << set.FindMinR("d3_1").f << std::endl;
	std::cout << "Orientff 0 D3_2: " << set.FindMinR("d3_2").R << "\t" << set.FindMinR("d3_2").f << std::endl;
	std::cout << "dr: " << set.FindMinR("d3_2").R - set.FindMinR("d3_1").R << "\t" << set.FindMinR("d3_2").f - set.FindMinR("d3_1").f << std::endl << "----------" << std::endl;
	
	std::cout << "Orientff 90 D2_1: " << set.FindMinR("d2_1").R << "\t" << set.FindMinR("d2_1").f << std::endl;
	std::cout << "Orientff 90 D2_2: " << set.FindMinR("d2_2").R << "\t" << set.FindMinR("d2_2").f << std::endl;
	std::cout << "dr: " << set.FindMinR("d2_2").R - set.FindMinR("d2_1").R << "\t" << set.FindMinR("d2_2").f - set.FindMinR("d2_1").f << std::endl << "----------" << std::endl;
		
	std::cout << "Orientff * D1_1: " << set.FindMinR("d1_1").R << "\t" << set.FindMinR("d1_1").f << std::endl;
	std::cout << "Orientff * D1_2: " << set.FindMinR("d1_2").R << "\t" << set.FindMinR("d1_2").f << std::endl;
	std::cout << "dr: " << set.FindMinR("d1_2").R - set.FindMinR("d1_1").R << "\t" << set.FindMinR("d1_2").f - set.FindMinR("d1_1").f << std::endl << "----------" << std::endl;
}

void Graph_DependenceFromOrientCNT(bool draw_graph = false) {
	wg::SampleSet set = wg::SampleSet(exp_dir + "orient.set", "s2p", *file_ex);
	if(draw_graph) {
		set.DrawSpectrR("OrientCNT 90 degree", {"d6_1", "d6_2"});
		set.DrawSpectrR("OrientCNT 45 degree", {"d5_1", "d5_2"});
		set.DrawSpectrR("OrientCNT 0 degree", {"d4_1", "d4_2"});
		set.DrawSpectrR("OrientCNT * degree", {"d7_1", "d7_2"});
		set.DrawSpectrR("OrientCNT without CNT", {"d0_1", "d0_2"});
	}
	std::cout << "============Orientation_CNT===========" << std::endl;
	std::cout << "without cnt D0_1: " << set.FindMinR("d0_1").R << "\t" << set.FindMinR("d0_1").f << std::endl;
	std::cout << "without cnt D0_2: " << set.FindMinR("d0_2").R << "\t" << set.FindMinR("d0_2").f << std::endl;
	std::cout << "dr: " << set.FindMinR("d0_2").R - set.FindMinR("d0_1").R << "\t" << set.FindMinR("d0_2").f - set.FindMinR("d0_1").f << std::endl << "----------" << std::endl;
	
	std::cout << "OrientCNT 0 D4_1: " << set.FindMinR("d4_1").R << "\t" << set.FindMinR("d4_1").f << std::endl;
	std::cout << "OrientCNT 0 D4_2: " << set.FindMinR("d4_2").R << "\t" << set.FindMinR("d4_2").f << std::endl;
	std::cout << "dr: " << set.FindMinR("d4_2").R - set.FindMinR("d4_1").R << "\t" << set.FindMinR("d4_2").f - set.FindMinR("d4_1").f << std::endl << "----------" << std::endl;
	
	std::cout << "OrientCNT 45 D5_1: " << set.FindMinR("d5_1").R << "\t" << set.FindMinR("d5_1").f << std::endl;
	std::cout << "OrientCNT 45 D5_2: " << set.FindMinR("d5_2").R << "\t" << set.FindMinR("d5_2").f << std::endl;
	std::cout << "dr: " << set.FindMinR("d5_2").R - set.FindMinR("d5_1").R << "\t" << set.FindMinR("d5_2").f - set.FindMinR("d5_1").f << std::endl << "----------" << std::endl;
	
	std::cout << "OrientCNT 90 D6_1: " << set.FindMinR("d6_1").R << "\t" << set.FindMinR("d6_1").f << std::endl;
	std::cout << "OrientCNT 90 D6_2: " << set.FindMinR("d6_2").R << "\t" << set.FindMinR("d6_2").f << std::endl;
	std::cout << "dr: " << set.FindMinR("d6_2").R - set.FindMinR("d6_1").R << "\t" << set.FindMinR("d6_2").f - set.FindMinR("d6_1").f << std::endl << "----------" << std::endl;
	
	std::cout << "OrientCNT * D7_1: " << set.FindMinR("d7_1").R << "\t" << set.FindMinR("d7_1").f << std::endl;
	std::cout << "OrientCNT * D7_2: " << set.FindMinR("d7_2").R << "\t" << set.FindMinR("d7_2").f << std::endl;
	std::cout << "dr: " << set.FindMinR("d7_2").R - set.FindMinR("d7_1").R << "\t" << set.FindMinR("d7_2").f - set.FindMinR("d7_1").f << std::endl << "----------" << std::endl;
}

void Graph_Append() {
	wg::SampleSet test_set = wg::SampleSet(exp_dir + "append.set", "s2p", *file_ex);
	test_set.DrawSpectrR("Comp", {"d8_1", "d8_2"});
	test_set.DrawSpectrR("Comp", {"d9_1", "d9_2"});
	test_set.DrawSpectrR("Cnt", {"d10_1", "d10_2"});
	
	test_set.DrawSpectrR("Cnt", {"d11_1", "d11_2"});
	test_set.DrawSpectrR("OrientFF", {"d12_1", "d12_2"});
	test_set.DrawSpectrR("Epox", {"d13_1", "d13_2"});
}

void DataXYInFile(std::string file_name, const DataXY& data) {
	std::ofstream os(exp_dir + "/depend/" + file_name);
	os << data;
}

void Graph_Dependence(std::string set_name, std::string param, std::initializer_list<std::string> field_literas, std::initializer_list<std::string> without_field_literas, bool draw_graph = false) {
	wg::SampleSet set = wg::SampleSet(exp_dir + set_name, "s2p", *file_ex);
	DataXY in_field = set.ParametrDependence(param, [](const auto& sample) {
				return wg::calc::FindMinR(sample->GetSpectrR()).R;
			}, field_literas
		);
	DataXY without_field = set.ParametrDependence(param, [](const auto& sample) {
				return wg::calc::FindMinR(sample->GetSpectrR()).R;
			}, 
			without_field_literas
		);
		
	DataXY dR = in_field - without_field;
	DataXY in_fieldF = set.ParametrDependence(param, [](const auto& sample) {
				return wg::calc::FindMinR(sample->GetSpectrR()).f;
			}, 
			field_literas
		);
	DataXY without_fieldF = set.ParametrDependence(param, [](const auto& sample) {
				return wg::calc::FindMinR(sample->GetSpectrR()).f;
			}, 
			without_field_literas
		);
	DataXY dF = in_fieldF - without_fieldF;
		
	std::cout << "============" << param << " Dependence===========" << std::endl;
	std::cout << "---R in field---" << std::endl;
	std::cout << in_field << std::endl;
	DataXYInFile(param + "_R_in", in_field);
	
	std::cout << "---R without field---" << std::endl;
	std::cout << without_field << std::endl;
	DataXYInFile(param + "_R_without", without_field);
	
	std::cout << "---dR---" << std::endl;
	std::cout << dR << std::endl;
	DataXYInFile(param + "_dR", dR);
	
	std::cout << "---F in field---" << std::endl;
	std::cout << in_fieldF << std::endl;
	DataXYInFile(param + "_F_in", in_fieldF);
	
	std::cout << "---F without field---" << std::endl;
	std::cout << without_fieldF << std::endl;
	DataXYInFile(param + "_F_without", without_fieldF);
	
	std::cout << "---dF in field---" << std::endl;
	std::cout << dF << std::endl;
	DataXYInFile(param + "_dF", dF);
	
	if(draw_graph) {	
		auto plot = wg::SpectrDrawer("From " + param + " R", *file_ex);
		plot.Add("In field", in_field);
		plot.Add("Without field", without_field);	
		plot.Draw();
		
		auto plotf = wg::SpectrDrawer("From " + param + " F", *file_ex);
		plotf.Add("In field", in_fieldF);
		plotf.Add("Without field", without_fieldF);	
		plotf.Draw();
		
		auto plot_dr = wg::SpectrDrawer("From " + param + " dR", *file_ex);
		plot_dr.Add("dR", in_field - without_field);
		plot_dr.Draw();
		auto plot_df = wg::SpectrDrawer("From " + param + " dF", *file_ex);
		plot_df.Add("dF", in_fieldF - without_fieldF);
		plot_df.Draw();
	}
}

wg::CalibratorSpectrR CalibrateOnFp(const std::string& file = "data/fp_3.95.s2p", bool show_graph = false) {
	auto st_fp = DislocStruct(wg::materials::Epsilon {2.0, 0});
	wg::CalibratorSpectrR calibrator = wg::CalibratorSpectrR(*file_ex);
	calibrator.CalibF(st_fp, file, show_graph);
	calibrator.CalibR(st_fp, file, show_graph);
	
	if(show_graph) {
		auto plot = wg::SpectrDrawer("Adjust result: ", *file_ex);
			plot.Add("before", wg::calc::BuildSpectrR(wg::calc::make_fv(9'400'000'000, 9'800'000'000, 1000000), st_fp));
			calibrator.Adjust(st_fp);
			plot.Add("after", wg::calc::BuildSpectrR(wg::calc::make_fv(9'400'000'000, 9'800'000'000, 1000000), st_fp));
			plot.Add("experiment", "data/fp_3.95.s2p");
		plot.Draw();
	}
	return calibrator;
}

void Permutation() {
	wg::f_vector_load fv = wg::calc::make_fv(9'400'000'000, 9'500'000'000, 1'000'000);
	double_long w = wg::calc::w(9'462'500'000);
	std::vector<wg::perm::MaskAlongX> mask = {{wg::waveguide_23x10.a/2, 0.002}};
	
	wg::perm::PermutatorAlongX permutation(
		mask, 
		w, 
		wg::materials::CreateWithParams(2.0, 0.0), 
		wg::materials::CreateWithParams(62.0, 12), 
		0.0001, 
		wg::waveguide_23x10
	); 
	
	auto result = permutation.CalcEps_n(stat_analize::BackTaskParams<double_long> {2, 5, 0.01, 100}, stat_analize::BackTaskParams<double_long> {0, 1, 0.001, 100});
	std::cout << "eps: " << result.val1 << ", " << result.val2 << std::endl;
	std::cout << "s: " << result.s << std::endl;
	/*	
	auto ps1 = DislocStruct(wg::materials::Epsilon{2.0, 0.0});
	auto ps2 = DislocStruct(wg::materials::Epsilon{result.val1, result.val2});

	auto plot = wg::SpectrDrawer("Permutation", *file_ex);
	plot.Add("before", ps1, fv);
	plot.Add("after", ps2, fv);
	plot.Draw();
	*/
	std::ofstream os("permut.surf");
	for(auto& d : result.surface) {
		os << d.val1 << "\t" << d.val2 << "\t" << std::log(d.s) << std::endl;
 	}
			
}

void Analize() {
	//Permutation();
	//wg::SampleSet test_set = wg::SampleSet("data/test.set", "s2p", file_ex);
	//test_set.DrawSpectrR("test", {"c10_1", "c10_2"});
	//auto calibrator = CalibrateOnFp("data/fp_3.95.s2p", true);
	
	//Graph_DependenceFromOrientFF();
	//Graph_DependenceFromOrientCNT();
	//Graph_Append();
	Graph_Dependence("field.set", "B", 
		{"c2_2", "b14_2", "b15_2", "b16_2", "b13_2",  "b17_2", "b12_2"}, 
		{"c2_1", "b14_1", "b15_1", "b16_1", "b13_1",  "b17_1", "b12_1"}, false
	);
	Graph_Dependence("comp.set", "Cv_comp", 
		{"c7_2", "c6_2", "c5_2", "c8_2"}, 
		{"c7_1", "c6_1", "c5_1", "c8_1"}, false
	);
	Graph_Dependence("cnt.set", "Cv_cnt", 
		{"c10_2", "c11_2", "c12_2", "c13_2", "c15_2", "c14_2"}, 
		{"c10_1", "c11_1", "c12_1", "c13_1", "c15_1", "c14_1"}, true
	);
	
	//algebra::vector::Vector<int> a {5, 3, -4}, b {6, 7, -8};
	//std::cout << (a ^ b) << std::endl; 
}

}
