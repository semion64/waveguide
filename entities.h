#pragma once

#include <iostream>
#include <complex>
#include <cmath>
#include <math.h>
#include <vector>
#include <string>

struct excp {
		excp() {
		}
		excp(std::string text) {
			std::cout << text;
		}
};

struct excp_incorrect_file_format : public excp {
	excp_incorrect_file_format() {}
	excp_incorrect_file_format(std::string text) : excp(text) { }
};

struct excp_file_not_found : public excp {
	excp_file_not_found() {}
	excp_file_not_found(std::string text) : excp(text) { }
};

typedef double double_long;
typedef std::complex<double_long> double_c;
const double dd = 0.00000001;
struct Point {
	double x, y;
	Point(double x, double y) {
		this->x = x;
		this->y = y;
	}
};

typedef std::vector<Point> DataXY;

struct SpectrRTValue {
	double f;
	double R;
	double T;
};

typedef std::vector<SpectrRTValue> SpectrRT;

struct Offset {
	double_long dx, dy;
	Offset(double_long dx, double_long dy) {
		this->dx = dx;
		this->dy = dy;
	}
};

namespace wg {	
const double_long PI = M_PI;
const double_long C = 299792458; //2.998 * (10 ^ 8); 
//const double_long MU_0 = 4 * PI * std::pow(10, -7);
//const double_long EPS_0 = 1 / (MU_0 * std::pow(C, 2));
const double_long MU_0 = 1.257 * std::pow(10, -6);
const double_long EPS_0 = 8.85 * std::pow(10, -12); 

struct PointR {
	double_long R, f;
};


struct Waveguide {
		double_long a, b;
};
}
