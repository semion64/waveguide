#include <cmath>
#include <vector>

#include "entities.h"
#include "materials.h"
#include "vector.h"

namespace wg {
namespace perm {

struct excp_dx_more_than_segment{};

//const double_c const_D = (1.0, 1.0);

typedef algebra::vector::Vector<double_c> Vec;

double_long sgs_to_si_E = std::sqrt(4 * PI * EPS_0),
			sgs_to_si_H = std::sqrt(4 * PI * MU_0);

struct MaskAlongX {
	double x;
	double dx;
};

Vec E(double_long w, double_long x, const Waveguide& waveguide = waveguide_23x10) {
	double_c _y = - w * PI / (waveguide.a) * std::sin(PI * x / waveguide.a) * sgs_to_si_E;
	return {0, _y, 0};
}

Vec H(double_long x, double_c gam, double_c mu, const Waveguide& waveguide = waveguide_23x10) {
	double_c _x = /*- j_mnim **/ gam * PI / (mu * MU_0 * waveguide.a) * std::sin(PI * x / waveguide.a) * sgs_to_si_H;
	double_c _z = - j_mnim * PI * PI / (mu * MU_0 * waveguide.a * waveguide.a) * std::cos(PI * x / waveguide.a) * sgs_to_si_H;
	double_c _y = 0;
	return {_x, _y, _z};
}

class PermutatorAlongX {
public:
	PermutatorAlongX(const std::vector<MaskAlongX>& mask, double_long w, const wg::materials::Material& material_0, const wg::materials::Material& material_n,  double_long dx, Waveguide waveguide = waveguide_23x10) 
		: mask_(mask), w_(w), material_0_(material_0), material_n_(material_n), dx_(dx), waveguide_(waveguide) {
		gamma_0_ = wg::calc::gamma(w_, material_0_, waveguide_);
		dEps_ = (material_n_.eps - material_0_.eps).value(w_) * EPS_0;
		dMu_ = (material_n_.mu - material_0_.mu).value(w_) * MU_0;
		std::cout << "deps: " << dEps_ << ", dmu: " << dMu_ << std::endl;
		mu_n_ = material_n_.mu.value(w_);
		mu_0_ = material_0_.mu.value(w_);
	}
	
	double_long S(double_c gamma_n) {
		double_c s = gamma_n - gamma_0_ - w_ * integral_Sn(gamma_n) / integral_S0(gamma_n);
		
		std::cout << "IntS0:" << (integral_S0(gamma_n)) << "\t" << "IntSn:" <<  integral_Sn(gamma_n) << "\t /:" << w_ * integral_Sn(gamma_n) / integral_S0(gamma_n) << " S: " << s<<std::endl;
		return std::abs(s);
	}	
		
	stat_analize::BackTaskResult2<double_long> CalcEps_n(const stat_analize::BackTaskParams<double_long>& eps_real, const stat_analize::BackTaskParams<double_long>& eps_imag) {
		stat_analize::SurfaceS2<double_long> surface;
		
		double_long val_start_1 = eps_real.start;
		double_long val_end_1 = eps_real.end;
		double_long delta_1 = eps_real.delta;
		int N_1 = eps_real.N;
		
		double_long val_start_2 = eps_imag.start;
		double_long val_end_2 = eps_imag.end;
		double_long delta_2 = eps_imag.delta;
		int N_2 = eps_imag.N;
		
		double_long step_1 = (val_end_1 - val_start_1) / N_1;
		double_long step_2 = (val_end_2 - val_start_2) / N_2;
		
		double_long s_min = S(wg::calc::gamma(w_, wg::materials::CreateWithParams(val_start_1, val_start_2), waveguide_));
		double_c eps_min = {val_start_1, val_start_2};
		
		do {
			for(double_long val_1 = val_start_1; val_1 <= val_end_1; val_1 += step_1) {
				for(double_long val_2 = val_start_2; val_2 <= val_end_2; val_2 += step_2) {
					double_long s = S(wg::calc::gamma(w_, wg::materials::CreateWithParams(val_1, val_2), waveguide_));
					std::cout << val_1 << "\t" << val_2 << "\t" << s << std::endl;
					if(s < s_min) {
						s_min = s;
						eps_min = {val_1, val_2};
					}
					surface.push_back(stat_analize::PointS2{val_1, val_2, s});
				}
			}
			val_start_1 = eps_min.real() - step_1;
			val_end_1 = eps_min.real() + step_1;
			step_1 = (val_end_1 - val_start_1) / N_1;
			val_start_2 = eps_min.imag() - step_2;
			val_end_2 =  eps_min.imag() + step_2;
			step_2 = (val_end_2 - val_start_2) / N_2;
		} while (step_1 > delta_1 || step_2 > delta_2);
		
		return {eps_min.real(), eps_min.imag(), s_min, surface};
	}		
		
	double_c integral_Sn (double_c gamma_n) {
		double_c sum = 0;
		double_c dS =  dx_ * waveguide_.b; //segment.dx * waveguide_.b;
		
		auto gamma_rel = (gamma_n / gamma_0_);
		for(const auto& segment : mask_) {
			if(dx_ > segment.dx) {
				throw excp_dx_more_than_segment{};
			}
			
			auto end_x = segment.x + segment.dx;
			double_long x = segment.x;
			for(; x < end_x; x += dx_) {
				Vec E_0 = E(w_, x, waveguide_);
				Vec E_n = E_0;
				Vec H_0 = H(x, gamma_0_, mu_0_, waveguide_);
				Vec H_n = gamma_rel * H_0;
				sum += (dEps_ * (E_n * algebra::vector::conj(E_0)) + dMu_ * (H_n * algebra::vector::conj(H_0))) * dS;
			}
			
			// calc the rest of the segment if segment width (dx) not a multiple of dx_
			x -= dx_;
			auto d_rest = end_x - x;
			if(d_rest > 0) {
				dS = d_rest * waveguide_.b;
				Vec E_0 = E(w_, x, waveguide_);
				Vec E_n = E_0;
				Vec H_0 = H(x, gamma_0_, mu_0_, waveguide_);
				Vec H_n = gamma_rel * H_0;
				sum += (dEps_ * (E_n * algebra::vector::conj(E_0)) + dMu_ * (H_n * algebra::vector::conj(H_0))) * dS;
			}
		}
		
		return sum;
	}

	double_c integral_S0 (double_c gamma_n) {
		//Vec sum {0, 0, 0};
		double_c  sum = 0;
		auto end_x = waveguide_.a;
		double_c dS = dx_ * waveguide_.b;
		auto gamma_rel = (gamma_n / gamma_0_);
		double_long x = 0;
		for(; x < end_x; x += dx_) {
			Vec E_0 = E(w_, x, waveguide_);
			Vec E_n = E_0;
			Vec H_0 = H(x, gamma_0_, mu_0_, waveguide_);
			Vec H_n = gamma_rel * H_0;
			
			//std::cout << E_n << "\t" << E_0 << "\t" << H_0 << "\t" << H_n << std::endl;
			sum += (((algebra::vector::conj(E_0) ^ H_n) + (E_n ^ algebra::vector::conj(H_0))).z * dS);
		}
		
		// calc the rest of the segment if waveguide.a not a multiple of dx_
		x -= dx_;
		auto d_rest = end_x - x;
		if(d_rest > 0) {
			dS = d_rest * waveguide_.b;
			Vec E_0 = E(w_, x, waveguide_);
			Vec E_n = E_0;
			Vec H_0 = H(x, gamma_0_, mu_0_, waveguide_);
			Vec H_n = gamma_rel * H_0;
			
			sum += (((algebra::vector::conj(E_0) ^ H_n) + (E_n ^ algebra::vector::conj(H_0))).z * dS);
		}
		
		return sum;
	}
	
	
	std::vector<MaskAlongX> mask_;
	double_long w_;
	double_long dx_;
	wg::materials::Material material_0_;
	wg::materials::Material material_n_;
	Waveguide waveguide_;
	
	double_c gamma_0_;//, gamma_1_;
	double_c dEps_, dMu_, mu_n_, mu_0_;	
	
	double_c S2(double_c gamma_n) {
		double_c s = gamma_n - gamma_0_ - w_ * integral_Sn(gamma_n) / integral_S0(gamma_n);
		
		std::cout << "IntS0:" << (integral_S0(gamma_n)) << "\t" << "IntSn:" <<  integral_Sn(gamma_n) << "\t /:" << w_ * integral_Sn(gamma_n) / integral_S0(gamma_n) << " S: " << s<<std::endl;
		return {std::abs(s.real()), std::abs(s.imag())};
	}
	

	double_c CalcEps_n2(const stat_analize::BackTaskParams<double_long>& bt_param_1, const stat_analize::BackTaskParams<double_long>& bt_param_2) {
		//std::vector<PointS2<Val>> s_PointS;
		double_long val_start_1 = bt_param_1.start;
		double_long val_end_1 = bt_param_1.end;
		double_long delta_1 = bt_param_1.delta;
		int N_1 = bt_param_1.N;
		double_long step_1 = (val_end_1 - val_start_1) / N_1;
		
		double_long val_start_2 = bt_param_2.start;
		double_long val_end_2 = bt_param_2.end;
		double_long delta_2 = bt_param_2.delta;
		int N_2 = bt_param_2.N;
		double_long step_2 = (val_end_2 - val_start_2) / N_2;
		
		double_c s_min = S2(wg::calc::gamma(w_, wg::materials::CreateWithParams(val_start_1, val_start_2), waveguide_));
		double_c g_min = {val_start_1, val_start_2};
		do {
			//PointS2<Val> min_before = min;
			for(double_long val_1 = val_start_1; val_1 <= val_end_1; val_1 += step_1) {
				for(double_long val_2 = val_start_2; val_2 <= val_end_2; val_2 += step_2) {
					double_c s = S2(wg::calc::gamma(w_, wg::materials::CreateWithParams(val_1, val_2), waveguide_));
					//std::cout << val_1 << "\t" << val_2 << "\t" << s << std::endl;
					if(s.real() < s_min.real()) {
						s_min.real(s.real());
						g_min.imag(val_2);
					}
					if(s.imag() < s_min.imag()) {
						s_min.imag(s.imag());
						g_min.real(val_1);
					}
					//s_PointS.push_back(PointS2{val_1, val_2, s});
				}
			}
			val_start_1 = g_min.real() - step_1;
			val_end_1 = g_min.real() + step_1;
			step_1 = (val_end_1 - val_start_1) / N_1;
			val_start_2 = g_min.imag() - step_2;
			val_end_2 =  g_min.imag() + step_2;
			step_2 = (val_end_2 - val_start_2) / N_2;
		} while (step_1 > delta_1 || step_2 > delta_2);
		return g_min;
		//return { min.val1, min.val2, min.s, s_PointS };
	}	
};
}
}


		

	
