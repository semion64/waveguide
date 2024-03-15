#pragma once

#include <complex>
#include <iostream>
#include <cmath>
#include <math.h>
#include <cassert>
#include <vector>
#include <map>
#include "entities.h"

namespace stat_analize {

struct excp_minimum_not_found {};

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

template <typename FuncS>
std::pair<double_long, double_long> BackTask(double_long val_start, double_long val_end, int N, double_long delta, FuncS func_s) {
	double_long step = (val_end - val_start) / N;
	std::pair<double_long, double_long> min {val_start, func_s(val_start)};
	do {
		std::pair<double_long, double_long> min_before {min.first, min.second};

		for(double_long val = val_start; val <= val_end; val += step) {
			double_long s = func_s(val);
			if(s < min.second) {
				min.first = val;
				min.second = s;
			}
		}
		
		if(std::abs(min.first - min_before.first) < dd) {
			throw excp_minimum_not_found {};
		}
		
		val_start = min.first - step;
		val_end = min.first + step;
		step = (val_end - val_start) / N;
	} while (step > delta);
	
	return min;
}
			

Point FindMinY(DataXY data, Offset offset = {0, 0});
}
