#include "transmission_matrix.h"

namespace wg{
TransmissionMatrix::TransmissionMatrix (double_long w, double_long z, Waveguide waveguide, materials::Material left, materials::Material right) {
	double_c g = calc::gamma(w, left, waveguide);
	double_c g1 = calc::gamma(w, right, waveguide);
	double_long two = 2.0;
	t_11 = (g1 + g)	/ (two * g1) * std::exp((g1 - g) * z);
	t_12 = (g1 - g)	/ (two * g1) * std::exp((g1 + g) * z);
	t_21 = (g1 - g)	/ (two * g1) * std::exp(-(g1 + g) * z);
	t_22 = (g1 + g)	/ (two * g1) * std::exp(-(g1 - g) * z);
}

TransmissionMatrix::TransmissionMatrix (double_c t11, double_c t12, double_c t21, double_c t22) {
	t_11 = t11; 
	t_12 = t12;
	t_21 = t21;
	t_22 = t22;
}

TransmissionMatrix::TransmissionMatrix () {
}

double_long TransmissionMatrix::R() {
	return std::pow(std::abs(-t_21/t_22), 2);
}

double_long TransmissionMatrix::T() {
	return std::pow(
			std::abs(
				(t_11 * t_22 - t_12 * t_21) 
				/ 
				t_22
			), 2);
}
TransmissionMatrix operator* (const TransmissionMatrix& l, const TransmissionMatrix& r) {
	double_c t11, t12, t21, t22;
	t11 = l.t_11 * r.t_11 + l.t_12 * r.t_21; 
	t21 = l.t_21 * r.t_11 + l.t_22 * r.t_21; 
	t12 = l.t_11 * r.t_12 + l.t_12 * r.t_22; 
	t22 = l.t_21 * r.t_12 + l.t_22 * r.t_22; 
	
	//std::cout <<  l.t_11 << t12 << t21 << t22 << std::endl;
	return TransmissionMatrix(t11, t12, t21, t22);
}

}
