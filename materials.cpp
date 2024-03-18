#include "materials.h"

namespace wg {
namespace materials {

Material GetMaterial(std::string material_name){
	return Material{Epsilon{0,0}, Mu{0,0}};
}

Material CreateWithParams(double_long eps_real, double_long eps_image, double_long mu_real, double_long mu_image){
	return Material{Epsilon{eps_real, eps_image}, Mu{mu_real, mu_image}};
}

Epsilon& operator+=(Epsilon& lhs, const Epsilon& rhs) {
	lhs.image += rhs.image;
	lhs.real += rhs.real;
	return lhs;
}

Epsilon operator+(const Epsilon& lhs, const Epsilon& rhs) {
	return {lhs.real + rhs.real, lhs.image + rhs.image };
}

Epsilon operator-(const Epsilon& lhs, const Epsilon& rhs) {
	return {rhs.real - lhs.real, rhs.image - lhs.image };
}

Mu& operator+=(Mu& lhs, const Mu& rhs) {
	lhs.image += rhs.image;
	lhs.real += rhs.real;
	return lhs;
}

Mu operator+(const Mu& lhs, const Mu& rhs) {
	return {lhs.real + rhs.real, lhs.image + rhs.image };
}

Mu operator-(const Mu& lhs, const Mu& rhs) {
	return {rhs.real - lhs.real, rhs.image - lhs.image };
}


}
}
