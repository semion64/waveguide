#pragma once

#include <complex>
#include <iostream>
#include <cmath>
#include <math.h>
#include <cassert>
#include <vector>
#include <map>
#include <tuple>
#include "entities.h"

namespace stat_analize {

struct excp_minimum_not_found {};

template <typename Val>
struct PointS {
	Val val;
	double_long s;
	PointS(Val val, double_long s) : val(val), s(s) { }
};

template <typename Val>
struct PointS2 {
	Val val1;
	Val val2;
	double_long s;
	PointS2(Val val1, Val val2, double_long s) : val1(val1), val2(val2), s(s) { }
};

template<typename Val>
struct BackTaskParams {
	Val start;
	Val end;
	Val delta;
	int N;
};

template<typename Val>
using DataS = std::vector<PointS<Val>>;
template<typename Val>
using BackTaskResult = std::tuple<Val, double_long, DataS<Val>>;

template<typename Val>
using DataS2 = std::vector<PointS2<Val>>;

template<typename Val>
using BackTaskResult2 = std::tuple<Val, Val, double_long, DataS2<Val>>;

template<typename Val>
DataXY PointSToDataXY(std::vector<PointS<Val>> pts) {
	DataXY d;
	for(auto& pt : pts) {
		d.emplace_back(pt.val, pt.s);
	}
	return d;
}

template <typename FuncY>
void BuildChartXY(std::ostream& os, double_long x_begin, double_long x_end, double_long x_step, FuncY y, Offset offset = {0, 0}) {
	for(double_long x = x_begin; x < x_end; x += x_step) {
		os << x + + offset.dx << "\t" << y(x) + offset.dy << std::endl;
	}
}

template <typename FuncY>
DataXY BuildChartXY(double_long x_begin, double_long x_end, double_long x_step, FuncY y, Offset offset = {0, 0}) {
	DataXY g;
	for(double_long x = x_begin; x < x_end; x += x_step) {
		g.emplace_back(x + offset.dx, y(x) + offset.dy);
	}
	
	return g;
}

template <typename FuncY>
void BuildChartXY(std::ostream& os, const std::vector<double>& x_vector, FuncY y, Offset offset = {0, 0}) {
	for(const auto& x : x_vector) {
		os << x + offset.dx << "\t" << y(x) + offset.dy << std::endl;
	}
}

template <typename FuncY>
DataXY BuildChartXY(const std::vector<double>& x_vector, FuncY y, Offset offset = {0, 0}) {
	DataXY g;
	for(const auto& x : x_vector) {
		g.emplace_back(x + offset.dx, y(x) + offset.dy);
	}
	
	return g;
}

template <typename FuncY>
Point FindMinY(double_long x_begin, double_long x_end, double_long x_step, FuncY y, Offset offset = {0, 0}) {
	Point min = {x_begin + offset.dx, y(x_begin) + offset.dy};
	for(double_long x = x_begin + x_step; x < x_end; x += x_step) {
		auto y0 = y(x) + offset.dy;
		if(y0 < min.y) {
			min.x = x + offset.dx;
			min.y = y0;
		}
	}
	
	return min;
}

template <typename FuncY>
Point FindMinY(const std::vector<double>& x_vector, FuncY y, Offset offset = {0, 0}) {
	assert(x_vector.size() > 0);
	Point min = {x_vector[0] + offset.dx, y(x_vector[0]) + offset.dy};
	for(auto x : x_vector) {
		auto y0 = y(x) + offset.dy;
		if(y0 < min.y) {
			min.x = x + offset.dx;
			min.y = y0;
		}
	}
	
	return min;
}

template <typename Val, typename FuncS>
BackTaskResult<Val> BackTask(const BackTaskParams<Val>& bt_param, FuncS func_s) {
	Val val_start = bt_param.start;
	Val val_end = bt_param.end;
	Val delta = bt_param.delta;
	Val N = bt_param.N;
	double_long step = (val_end - val_start) / N;
	std::vector<PointS<Val>> s_points;
	PointS<Val> min {val_start, func_s(val_start)};
	do {
		//PointS<Val> min_before = min;

		for(Val val = val_start; val <= val_end; val += step) {
			double_long s = func_s(val);
			if(s < min.s) {
				min = {val, s};
			}
			
			s_points.push_back(PointS{val, s});
		}
		
		/*if(min.val == min_before.val) {
			throw excp_minimum_not_found {};
		}*/
		
		val_start = min.val - step;
		val_end = min.val + step;
		step = (val_end - val_start) / N;
	} while (step > delta);
	
	return { min.val, min.s, s_points };
}

template <typename Val, typename FuncS>
BackTaskResult2<Val> BackTask(const BackTaskParams<Val>& bt_param_1, const BackTaskParams<Val>& bt_param_2, FuncS func_s) {
	std::vector<PointS2<Val>> s_points;
	
	Val val_start_1 = bt_param_1.start;
	Val val_end_1 = bt_param_1.end;
	Val delta_1 = bt_param_1.delta;
	Val N_1 = bt_param_1.N;
	double_long step_1 = (val_end_1 - val_start_1) / N_1;
	
	Val val_start_2 = bt_param_2.start;
	Val val_end_2 = bt_param_2.end;
	Val delta_2 = bt_param_2.delta;
	Val N_2 = bt_param_2.N;
	double_long step_2 = (val_end_2 - val_start_2) / N_2;
	
	PointS2<Val> min {val_start_1, val_start_2, func_s(val_start_1, val_start_2)};
	do {
		//PointS2<Val> min_before = min;
		/*if(step_1 > delta_1 && step_2 > delta_2)
		{*/
			for(Val val_1 = val_start_1; val_1 <= val_end_1; val_1 += step_1) {
				for(Val val_2 = val_start_2; val_2 <= val_end_2; val_2 += step_2) {
					double_long s = func_s(val_1, val_2);
					if(s < min.s) {
						min = {val_1, val_2, s};
					}
					s_points.push_back(PointS2{val_1, val_2, s});
				}
			}
		/*}
		else if(step_1 > delta_1) {
			for(Val val_1 = val_start_1; val_1 <= val_end_1; val_1 += step_1) {
				double_long s = func_s(val_1, min.val2);
				if(s < min.s) {
					min = {val_1, min.val2, s};
				}
				s_points.push_back(PointS2{val_1, min.val2, s});
			}
		}
		else if(step_2 > delta_2) {
			for(Val val_2 = val_start_2; val_2 <= val_end_2; val_2 += step_2) {
				double_long s = func_s(min.val1, val_2);
				if(s < min.s) {
					min = {min.val1, val_2, s};
				}
				
				s_points.push_back(PointS2{min.val1, val_2, s});
			}
		}*/
		
		/*if(min.val_1 == min_before.val_1 && min.val_2 == min_before.val_2) {
			throw excp_minimum_not_found {};
		}*/
		//if(step_1 > delta_1) {
			val_start_1 = min.val1 - step_1;
			val_end_1 = min.val1 + step_1;
			step_1 = (val_end_1 - val_start_1) / N_1;
		//}
		
		//if(step_2 > delta_2) {
			val_start_2 = min.val2 - step_2;
			val_end_2 = min.val2 + step_2;
			step_2 = (val_end_2 - val_start_2) / N_2;
		//}
	} while (step_1 > delta_1 || step_2 > delta_2);
	
	return { min.val1, min.val2, min.s, s_points };
}		

Point FindMinY(const DataXY& data, Offset offset = {0, 0});
}
