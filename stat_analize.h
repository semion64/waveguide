#pragma once

#include <complex>
#include <iostream>
#include <cmath>
#include <math.h>
#include <cassert>
#include <vector>

#include "entities.h"

namespace stat_analize {
	
template <typename FuncY>
void BuildChartXY(std::ostream& os, double_long x_begin, double_long x_end, double_long x_step, FuncY y) {
	for(double_long x = x_begin; x < x_end; x += x_step) {
		os << x << "\t" << y(x) << std::endl;
	}
}

template <typename FuncY>
DataXY BuildChartXY(double_long x_begin, double_long x_end, double_long x_step, FuncY y) {
	DataXY g;
	for(double_long x = x_begin; x < x_end; x += x_step) {
		g.emplace_back(x, y(x));
	}
	
	return g;
}

template <typename FuncY>
void BuildChartXY(std::ostream& os, const std::vector<double>& x_vector, FuncY y) {
	for(const auto& x : x_vector) {
		os << x << "\t" << y(x) << std::endl;
	}
}

template <typename FuncY>
DataXY BuildChartXY(const std::vector<double>& x_vector, FuncY y) {
	DataXY g;
	for(const auto& x : x_vector) {
		g.emplace_back(x, y(x));
	}
	
	return g;
}

template <typename FuncY>
Point FindMinY(double_long x_begin, double_long x_end, double_long x_step, FuncY y) {
	Point min = {x_begin, y(x_begin)};
	for(double_long x = x_begin + x_step; x < x_end; x += x_step) {
		auto y0 = y(x);
		if(y0 < min.y) {
			min.x = x;
			min.y = y0;
		}
	}
	
	return min;
}

template <typename FuncY>
Point FindMinY(const std::vector<double>& x_vector, FuncY y) {
	assert(x_vector.size() > 0);
	Point min = {x_vector[0], y(x_vector[0])};
	for(auto x : x_vector) {
		auto y0 = y(x);
		if(y0 < min.y) {
			min.x = x;
			min.y = y0;
		}
	}
	
	return min;
}
}
