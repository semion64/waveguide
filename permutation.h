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

struct MaskAlongX {
	double x;
	double dx;
};

Vec E(double_long w, double_long x, const Waveguide& waveguide = waveguide_23x10) {
	double_c _y = - w * PI / (waveguide.a) * std::sin(PI * x / waveguide.a);
	return {0, _y, 0};
}

Vec H(double_long x, double_c gam, double_c mu, const Waveguide& waveguide = waveguide_23x10) {
	double_c _x = - j_mnim * gam * PI / (mu * MU_0 * waveguide.a) * std::sin(PI * x / waveguide.a);
	double_c _z = - j_mnim * PI * PI / (mu * MU_0 * waveguide.a * waveguide.a) * std::cos(PI * x / waveguide.a);
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
		std::cout << dEps_ << " " << dMu_ << std::endl;
		mu_n_ = material_n_.mu.value(w_);
		mu_0_ = material_n_.mu.value(w_);
	}
	double_long S(double_c gamma_n) {
		std::cout << "IntS0:" << (integral_S0(gamma_n)) << "\t" << "IntSn:" <<  integral_Sn(gamma_n) << "\t /:" << w_ * integral_Sn(gamma_n) / integral_S0(gamma_n) << std::endl;
		double_c s = gamma_n - gamma_0_ - w_ * integral_Sn(gamma_n) / integral_S0(gamma_n);
		return std::abs(s);
	}	
	double_c CalcGamma_n(const stat_analize::BackTaskParams<double_long>& bt_param_1, const stat_analize::BackTaskParams<double_long>& bt_param_2) {
		//std::vector<PointS2<Val>> s_points;
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
		
		double_long s_min = S({val_start_1, val_start_2});
		double_c g_min = {val_start_1, val_start_2};
		do {
			//PointS2<Val> min_before = min;
			/*if(step_1 > delta_1 && step_2 > delta_2)
			{*/
				for(double_long val_1 = val_start_1; val_1 <= val_end_1; val_1 += step_1) {
					for(double_long val_2 = val_start_2; val_2 <= val_end_2; val_2 += step_2) {
						double_long s = S({val_1, val_2});
						std::cout << val_1 << "\t" << val_2 << "\t" << s << std::endl;
						if(s < s_min) {
							s_min = s;
							g_min = {val_1, val_2};
						}
						//s_points.push_back(PointS2{val_1, val_2, s});
					}
				}
			/*}
			else if(step_1 > delta_1) {
				for(Val val_1 = val_start_1; val_1 <= val_end_1; val_1 += step_1) {
					double_long s = func_s(val_1, min.val2);
					if(s < min.s) {
						min = {val_1, min.val2, s};
					}
					s_points.push_back(PointS2{val_1, min.val2, s});
				}
			}
			else if(step_2 > delta_2) {
				for(Val val_2 = val_start_2; val_2 <= val_end_2; val_2 += step_2) {
					double_long s = func_s(min.val1, val_2);
					if(s < min.s) {
						min = {min.val1, val_2, s};
					}
					
					s_points.push_back(PointS2{min.val1, val_2, s});
				}
			}*/
			
			/*if(min.val_1 == min_before.val_1 && min.val_2 == min_before.val_2) {
				throw excp_minimum_not_found {};
			}*/
			//if(step_1 > delta_1) {
				val_start_1 = g_min.real() - step_1;
				val_end_1 = g_min.real() + step_1;
				step_1 = (val_end_1 - val_start_1) / N_1;
			//}
			
			//if(step_2 > delta_2) {
				val_start_2 = g_min.imag() - step_2;
				val_end_2 =  g_min.imag() + step_2;
				step_2 = (val_end_2 - val_start_2) / N_2;
			//}
		} while (step_1 > delta_1 || step_2 > delta_2);
		return g_min;
		//return { min.val1, min.val2, min.s, s_points };
	}		
			
	/*double_c CalcGamma_n(const stat_analize::BackTaskParams<double_long>& bt_param_1, const stat_analize::BackTaskParams<double_long>& bt_param_2) {
		//std::vector<PointS2<Val>> s_points;
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
		
		double_c s_min = S({val_start_1, val_start_2});
		double_c g_min = {val_start_1, val_start_2};
		do {
			//PointS2<Val> min_before = min;
			
				for(double_long val_1 = val_start_1; val_1 <= val_end_1; val_1 += step_1) {
					for(double_long val_2 = val_start_2; val_2 <= val_end_2; val_2 += step_2) {
						double_c s = S({val_1, val_2});
						std::cout << val_1 << "\t" << val_2 << "\t" << s << std::endl;
						if(s.real() < s_min.real()) {
							s_min.real(s.real());
							g_min.real(val_1);
						}
						
						if(s.imag() < s_min.imag()) {
							s_min.imag(s.imag());
							g_min.imag(val_2);
						}
						
						//s_points.push_back(PointS2{val_1, val_2, s});
					}
				}
			
			else if(step_1 > delta_1) {
				for(Val val_1 = val_start_1; val_1 <= val_end_1; val_1 += step_1) {
					double_long s = func_s(val_1, min.val2);
					if(s < min.s) {
						min = {val_1, min.val2, s};
					}
					s_points.push_back(PointS2{val_1, min.val2, s});
				}
			}
			else if(step_2 > delta_2) {
				for(Val val_2 = val_start_2; val_2 <= val_end_2; val_2 += step_2) {
					double_long s = func_s(min.val1, val_2);
					if(s < min.s) {
						min = {min.val1, val_2, s};
					}
					
					s_points.push_back(PointS2{min.val1, val_2, s});
				}
			}
			
			
			//if(step_1 > delta_1) {
				val_start_1 = g_min.real() - step_1;
				val_end_1 = g_min.real() + step_1;
				step_1 = (val_end_1 - val_start_1) / N_1;
			//}
			
			//if(step_2 > delta_2) {
				val_start_2 = g_min.imag() - step_2;
				val_end_2 =  g_min.imag() + step_2;
				step_2 = (val_end_2 - val_start_2) / N_2;
			//}
		} while (step_1 > delta_1 || step_2 > delta_2);
		return g_min;
		//return { min.val1, min.val2, min.s, s_points };
	}		
		*/			
private:
	

	double_c integral_Sn (double_c gamma_n) {
		double_c sum = 0;
		
		auto gamma_rel = (gamma_n / gamma_0_);
		for(const auto& segment : mask_) {
			if(dx_ > segment.dx) {
				throw excp_dx_more_than_segment{};
			}
			
			double_c dS =  segment.dx * waveguide_.b;
			auto end_x = segment.x + segment.dx;
			double_long x = segment.x;
			for(; x < end_x; x += dx_) {
				Vec E_n = E(w_, x, waveguide_);
				Vec E_0 = E(w_, x, waveguide_);
				
				Vec H_0 = H(x, gamma_0_, mu_0_, waveguide_);
				Vec H_n = gamma_rel * H_0;
				sum += (dEps_ * (c2 * E_n * c2 * algebra::vector::conj(E_0)) + dMu_ * (H_n * algebra::vector::conj(H_0))) * dS;
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
				sum += (dEps_ * (c2 * E_n * c2 * algebra::vector::conj(E_0)) + dMu_ * (H_n * algebra::vector::conj(H_0))) * dS;
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
			sum += (((c2 * algebra::vector::conj(E_0) ^ H_n).module() + (c2 * E_n ^ algebra::vector::conj(H_0)).module()) * dS);
		}
		
		// calc the rest of the segment if waveguide.a not a multiple of dx_
		x -= dx_;
		auto d_rest = end_x - x;
		if(d_rest > 0) {
			dS = d_rest * waveguide_.b;
			Vec E_n = E(w_, x, waveguide_);
			Vec E_0 = E(w_, x, waveguide_);
			
			Vec H_0 = H(x, gamma_0_, mu_0_, waveguide_);
			Vec H_n = gamma_rel * H_0;
			
			sum += (((c2 * algebra::vector::conj(E_0) ^ H_n).module() + (c2 * E_n ^ algebra::vector::conj(H_0)).module()) * dS);
		}
		
		return sum;//.module();
	}
	
	
	std::vector<MaskAlongX> mask_;
	double_long w_;
	double_long dx_;
	wg::materials::Material material_0_;
	wg::materials::Material material_n_;
	Waveguide waveguide_;
	
	double_c gamma_0_;//, gamma_1_;
	double_c dEps_, dMu_, mu_n_, mu_0_;
	double_c c2 = C*C;
	
};
}
}
