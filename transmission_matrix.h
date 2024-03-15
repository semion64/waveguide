#pragma once

#include <vector>
#include <cmath>
#include "entities.h"
#include "structure.h"
#include "waveguide.h"

namespace wg{

class TransmissionMatrix {
public:	
	TransmissionMatrix (double_long w, double_long z, Waveguide waveguide,
							materials::Material left, materials::Material right);
	TransmissionMatrix (double_c t11, double_c t12, double_c t21, double_c t22);
	TransmissionMatrix();
	double_long R();
	double_long T();
	double_c t_11, t_12, t_21, t_22;
};

TransmissionMatrix operator* (const TransmissionMatrix& l, const TransmissionMatrix& r);
}
