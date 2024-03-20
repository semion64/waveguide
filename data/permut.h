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


wg::f_vector_load make_fv(double_long begin, double_long end, double_long step) {
	wg::f_vector_load fv;
	for(auto f = begin; f < end; f += step) {
		fv.push_back(f);
	}
	
	return fv;
}

wg::f_vector_load make_fv_N(double_long begin, double_long end, int N) {
	return make_fv(begin, end, (end - begin) / N);
}

void Analize() {
	
	wg::f_vector_load fv = make_fv(9'400'000'000, 9'500'000'000, 1'000'000);
	
	double_long w = wg::calc::w(9'462'500'000);
	//double_long w = 9'353'000'000;
	std::vector<wg::perm::MaskAlongX> mask = {{wg::waveguide_23x10.a/2, 0.003}};
	//PermutatorAlongX(const std::vector<MaskAlongX>& mask, double_long w, const wg::materials::Material& material_0, const wg::materials::Material& material_n,  double_long dx, Waveguide waveguide = waveguide_23x10) 
	
	wg::perm::PermutatorAlongX permutation(
		mask, 
		w, 
		wg::materials::CreateWithParams(2.0, 0.0), 
		wg::materials::CreateWithParams(2.0, 0.2), 
		0.0005, 
		wg::waveguide_23x10
	); 
	
	
	// stat_analize::DataS2<double_long> s_points;
	//auto g_n = permutation.CalcGamma_n(stat_analize::BackTaskParams<double_long> {0, 10, 0.0001,100}, stat_analize::BackTaskParams<double_long> {250, 256, 0.01, 100});
	
	auto eps = permutation.CalcEps_n(stat_analize::BackTaskParams<double_long> {2, 5, 0.001, 100}, stat_analize::BackTaskParams<double_long> {0, 1, 0.0001, 100});
	std::cout << "eps: " << eps << std::endl;
	
	
	auto ps1 = DislocStruct(wg::materials::Epsilon{2.0, 0.0});
	auto ps2 = DislocStruct(wg::materials::Epsilon{eps.real(), eps.imag()});

	auto plot = wg::SpectrDrawer("Permutation", *file_ex);
	plot.Add("before", ps1, fv);
	plot.Add("after", ps2, fv);
	plot.Draw();
			
}

}
