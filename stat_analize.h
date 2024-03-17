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


template<typename Val>
using BackTaskResult = std::tuple<Val, double_long, std::vector<PointS<Val>>>;

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
BackTaskResult<Val> BackTask(Val val_start, Val val_end, int N, Val delta, FuncS func_s) {
	double_long step = (val_end - val_start) / N;
	std::vector<PointS<Val>> s_points;
	PointS<Val> min {val_start, func_s(val_start)};
	do {
		PointS<Val> min_before = min;//{min.val, min.s};

		for(Val val = val_start; val <= val_end; val += step) {
			double_long s = func_s(val);
			if(s < min.s) {
				/*min.first = val;
				min.second = s;*/
				min = {val, s};
			}
			
			s_points.push_back(PointS{val, s});
		}
		
		if(min.val == min_before.val) {
			throw excp_minimum_not_found {};
		}
		
		val_start = min.val - step;
		val_end = min.val + step;
		step = (val_end - val_start) / N;
	} while (step > delta);
	
	return { min.val, min.s, s_points };
}
			

Point FindMinY(const DataXY& data, Offset offset = {0, 0});
}
