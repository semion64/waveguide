#include "waveguide.h"

namespace wg {

namespace calc {
f_vector_load DataXYToFVector(const DataXY& data) {
	f_vector_load fv;
	for(auto pt : data) {
		fv.push_back(pt.x);
	}
	return fv;
}
using namespace std::complex_literals;

double_long w(double_long f) {
		return 2 * PI * f;
}
double_long to_dbm(double_long R) {
	return 10 * std::log(R);
}

double_c gamma(double_long w, materials::Material material, Waveguide waveguide) {
	double_c a =
			(PI*PI) / (waveguide.a * waveguide.a)  
		- 
			w * w * EPS_0 * MU_0 * material.eps.value(w) * material.mu.value(w) + 0i;
	double_c b = std::sqrt(a);
	return b;
}	

double_long CalcR(double_long w, const std::vector<Layer>& structure, Waveguide waveguide) {
	std::vector<TransmissionMatrix> tm;
	double_long z = 0;
	for (int i = 0; i < structure.size() - 1; ++i) {
		tm.push_back(TransmissionMatrix(w, z, waveguide, structure[i].material, structure[i + 1].material));
		z += structure[i + 1].d;
	} 
	
	/*for(const auto& m : tm) {
		std::cout << m.t_11 << "\t" << m.t_12 << std::endl << m.t_21 << "\t" << m.t_22 << std::endl << std::endl;
	}*/
	
	TransmissionMatrix tm_res = tm[tm.size() - 1];
	for(int i = tm.size() - 2; i >= 0; --i){
		tm_res = tm_res * tm[i];
	}
	
	return tm_res.R();
}
/*
 void SpectrR(std::ostream& os, double_long f_begin, double_long f_end, double_long f_step, 
				const std::vector<Layer>& structure, Waveguide waveguide) {
	for(double_long f = f_begin; f < f_end; f += f_step) {
		double_long r = CalcR(w(f), structure, waveguide);
		os << f << "\t" << to_dbm(r) << std::endl;
	}
} */
 
void BuildSpectrR(std::ostream& os, double_long f_begin, double_long f_end, double_long f_step,	const Struct& st, Waveguide waveguide) {
	stat_analize::BuildChartXY(os, f_begin, f_end, f_step, [&st, &waveguide](double_long f) {return to_dbm(CalcR(w(f), st.GetAdjustStruct(), waveguide));}, st.GetOffset());
}

void BuildSpectrR(std::ostream& os, f_vector fv, const Struct& st, Waveguide waveguide) {
	stat_analize::BuildChartXY(os, fv, [&st, &waveguide](double_long f) {return to_dbm(CalcR(w(f), st.GetAdjustStruct(), waveguide));}, st.GetOffset());
}

DataXY BuildSpectrR(double_long f_begin, double_long f_end, double_long f_step, const Struct& st, Waveguide waveguide) {
	return stat_analize::BuildChartXY(f_begin, f_end, f_step, [&st, &waveguide](double_long f) {return to_dbm(CalcR(w(f), st.GetAdjustStruct(), waveguide));}, st.GetOffset());
}

DataXY BuildSpectrR(f_vector fv, const Struct& st, Waveguide waveguide) {
	return stat_analize::BuildChartXY(fv, [&st, &waveguide](double_long f) {return to_dbm(CalcR(w(f), st.GetAdjustStruct(), waveguide));}, st.GetOffset());
}

PointR FindMinR(double_long f_begin, double_long f_end, double_long f_step, const Struct& st, Offset offset, Waveguide waveguide) {
	Point min = stat_analize::FindMinY(f_begin, f_end, f_step, [&st, &waveguide](double_long f) {return to_dbm(CalcR(w(f), st.GetAdjustStruct(), waveguide));}, st.GetOffset());
	return PointR{min.y, min.x};
}

PointR FindMinR(f_vector fv, const Struct& st, Waveguide waveguide) {
	Point min = stat_analize::FindMinY(fv, [&st, &waveguide](double_long f) {return to_dbm(CalcR(w(f), st.GetAdjustStruct(), waveguide));}, st.GetOffset());
	return PointR{min.y, min.x};
}

PointR FindMinR(const DataXY& data,  Offset offset) {
	Point min = stat_analize::FindMinY(data, offset);
	return PointR{min.y, min.x};
}

}
}
