#pragma once

#include <string>
#include "entities.h"

namespace wg {
namespace materials {
	
struct Epsilon {
	double_long real;
	double_long image;
	double_c value(double_long w) {
		return double_c(real, -image);//-sigma / (EPS_0 * w));
	}
};

Epsilon& operator+=(Epsilon& lhs, const Epsilon& rhs);
Epsilon operator+(const Epsilon& lhs, const Epsilon& rhs);
Epsilon operator-(const Epsilon& lhs, const Epsilon& rhs);

struct Mu {
	double_long real;
	double_long image;
	double_c value(double_long w) {
		return double_c(real, image);
	}
};

Mu& operator+=(Mu& lhs, const Mu& rhs);
Mu operator+(const Mu& lhs, const Mu& rhs);
Mu operator-(const Mu& lhs, const Mu& rhs);

struct Material {
	Epsilon eps;
	Mu mu;
};

Material GetMaterial(std::string material_name);

Material CreateWithParams(double_long eps_real, double_long eps_imag = 0, double_long mu_real = 1, double_long mu_imag = 0);

}
}
