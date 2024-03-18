#include <cmath>
#include <vector>

#include "entities.h"
#include "materials.h"
#include "vector.h"

namespace wg {
namespace perm {

struct excp_dx_more_than_segment{};

const double_c const_D = (1.0, 0.0);

typedef algebra::vector::Vector<double_c> Vec;

struct MaskAlongX {
	double x;
	double dx;
};

Vec E(double_long w, double_long x, const Waveguide& waveguide = waveguide_23x10) {
	double_c _y = -const_D * w * PI / (waveguide.a) * std::sin(PI * x / waveguide.a);
	return {0, _y, 0};
}

Vec H(double_long x, double_c gam, double_c mu, const Waveguide& waveguide = waveguide_23x10) {
	double_c _x = -const_D * j_mnim * gam * PI / (mu * MU_0 * waveguide.a) * std::sin(PI * x / waveguide.a);
	double_c _z = -const_D * j_mnim * PI * PI / (mu * MU_0 * waveguide.a * waveguide.a) * std::cos(PI * x / waveguide.a);
	double_c _y = 0;
	return {_x, _y, _z};
}

class PermutatorAlongX {
public:
	PermutatorAlongX(const std::vector<MaskAlongX>& mask, double_long w, const wg::materials::Material& material_0, const wg::materials::Material& material_n,  double_long dx, Waveguide waveguide = waveguide_23x10) 
		: mask_(mask), w_(w), material_0_(material_0), material_n_(material_n), dx_(dx), waveguide_(waveguide) {
		gamma_0_ = wg::calc::gamma(w_, material_0_, waveguide_);
		dEps_ = (material_n_.eps - material_0_.eps).value(w_);
		dMu_ = (material_n_.mu - material_0_.mu).value(w_);
		mu_n_ = material_n_.mu.value(w_);
		mu_0_ = material_n_.mu.value(w_);
	}
	
	double_c S(double_c gamma_n) {
		return gamma_n - gamma_0_ - w_ * integral_Sn(gamma_n) / integral_S0(gamma_n);
	}

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
				sum += (dEps_ * (E_n * E_0) + dMu_ * (H_n * H_0)) * dS;
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
				sum += (dEps_ * (E_n * E_0) + dMu_ * (H_n * H_0)) * dS;
			}
		}
		
		return sum;
	}

	double_c integral_S0 (double_c gamma_n) {
		Vec sum {0, 0, 0};
		auto end_x = waveguide_.a;
		double_c dS = dx_ * waveguide_.b;
		auto gamma_rel = (gamma_n / gamma_0_);
		double_long x = 0;
		for(; x < end_x; x += dx_) {
			Vec E_0 = E(w_, x, waveguide_);
			Vec E_n = E_0;
			Vec H_0 = H(x, gamma_0_, mu_0_, waveguide_);
			Vec H_n = gamma_rel * H_0;
			
			sum += ((E_0 ^ H_n) + (E_n ^ H_0)) * dS;
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
			sum += ((E_0 ^ H_n) + (E_n ^ H_0)) * dS;
		}
		
		return sum.module();
	}
	
private:
	std::vector<MaskAlongX> mask_;
	double_long w_;
	double_long dx_;
	wg::materials::Material material_0_;
	wg::materials::Material material_n_;
	Waveguide waveguide_;
	
	double_c gamma_0_;//, gamma_1_;
	double_c dEps_, dMu_, mu_n_, mu_0_;
};
}
}
