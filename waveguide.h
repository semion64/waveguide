#pragma once

#include <complex>
#include <iostream>
#include <cmath>
#include <math.h>
#include "entities.h"
#include "materials.h"
#include "structure.h"
#include "transmission_matrix.h"
#include "stat_analize.h"

namespace wg {	
	
typedef std::vector<double> f_vector_load;
typedef const std::vector<double>& f_vector;

namespace calc {
	
f_vector_load DataXYToFVector(const DataXY& data);

double_long w(double_long f);

double_c gamma(double_long w, materials::Material material, Waveguide waveguide = waveguide_23x10);

double_long CalcR(double_long w, const std::vector<Layer>& structure, Waveguide waveguide = waveguide_23x10);
double_long to_dbm(double_long R);

void BuildSpectrR(std::ostream& os, double_long f_begin, double_long f_end, double_long f_step,	const Struct& structure, Waveguide waveguide = waveguide_23x10);
void BuildSpectrR(std::ostream& os, f_vector fv, const Struct& structure, Waveguide waveguide  = waveguide_23x10);

DataXY BuildSpectrR(double_long f_begin, double_long f_end, double_long f_step, const Struct& structure, Waveguide waveguide = waveguide_23x10);
DataXY BuildSpectrR(f_vector fv, const Struct& structure, Waveguide waveguide  = waveguide_23x10);
				
PointR FindMinR(double_long f_begin, double_long f_end, double_long f_step,	const Struct& structure, Waveguide waveguide = waveguide_23x10);
PointR FindMinR(f_vector fv, const Struct& structure,  Waveguide waveguide = waveguide_23x10);
PointR FindMinR(const DataXY& data, Offset offset = {0,0});

template <typename Val, typename StructParamFunc>
stat_analize::BackTaskResult<Val> BackTaskByPeackFreq(const DataXY& exp_data, wg::f_vector fv, const stat_analize::BackTaskParams<Val>& bt_param, StructParamFunc ps_param_func){
	wg::PointR min_exp = wg::calc::FindMinR(exp_data);
	return stat_analize::BackTask(bt_param,  
		[&min_exp, &fv, &ps_param_func](Val val) {
             return std::sqrt(
				std::pow(
					wg::calc::FindMinR(
						fv, ps_param_func(val)
					).f - min_exp.f, 2
				) / 2
			);
        });  
}

template <typename Val, typename StructParamFunc>
stat_analize::BackTaskResult<Val> BackTaskByPeackR(const DataXY& exp_data, wg::f_vector fv, const stat_analize::BackTaskParams<Val>& bt_param, StructParamFunc ps_param_func){
	wg::PointR min_exp = wg::calc::FindMinR(exp_data);
	return stat_analize::BackTask(bt_param,  
		[&min_exp, &fv, &ps_param_func](Val val) {
             return std::sqrt(
				std::pow(
					wg::calc::FindMinR(
						fv, ps_param_func(val)
					).R - min_exp.R, 2
				) / 2
			);
        });  
}	
	
}
}
