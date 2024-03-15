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
	
const Waveguide waveguide_23x10{0.023, 0.01};

typedef const std::vector<double> f_vector_load;
typedef const std::vector<double>& f_vector;

namespace calc {

double_long w(double_long f);

double_c gamma(double_long w, materials::Material material, Waveguide waveguide = waveguide_23x10);

double_long CalcR(double_long w, const std::vector<Layer>& structure, Waveguide waveguide = waveguide_23x10);
double_long to_dbm(double_long R);

void BuildSpectrR(std::ostream& os, double_long f_begin, double_long f_end, double_long f_step,	const PhotonStructure& structure, Offset offset = {0,0}, Waveguide waveguide = waveguide_23x10);
void BuildSpectrR(std::ostream& os, f_vector fv, const PhotonStructure& structure, Offset offset = {0,0}, Waveguide waveguide  = waveguide_23x10);

DataXY BuildSpectrR(double_long f_begin, double_long f_end, double_long f_step, const PhotonStructure& structure, Offset offset = {0,0}, Waveguide waveguide = waveguide_23x10);
DataXY BuildSpectrR(f_vector fv, const PhotonStructure& structure, Offset offset = {0,0}, Waveguide waveguide  = waveguide_23x10);
				
PointR FindMinR(double_long f_begin, double_long f_end, double_long f_step,	const PhotonStructure& structure, Offset offset = {0,0}, Waveguide waveguide = waveguide_23x10);
PointR FindMinR(f_vector fv, const PhotonStructure& structure, Offset offset = {0,0},  Waveguide waveguide = waveguide_23x10);
PointR FindMinR(DataXY data, Offset offset = {0,0});	
}
}