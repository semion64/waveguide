#include "entities.h"
#include "material.h"
#include <cmath>

namespace wg {
namespace perm {
const const_D = 1;
struct E {
	double_c x, y, z;
	
	E(double_long w, double_long x, Waveguide waveguide = waveguide_23x10) {
		y = - const_D * w * PI / (waveguide.a) * std::sin(PI * x / waveguide.a);
		x = 0;
		z = 0;
	}
};

struct H {
	double_c x, y, z;
	
	H(double_long x, double_c gam, double_c mu, Waveguide waveguide = waveguide_23x10) {
		x = -const_D * j_mnim * gam * PI / (mu * MU_0 * waveguide.a) * std::sin(PI * x / waveguide.a);
		z = -const_D * j_mnim * PI * PI / (mu * MU_0 * waveguide.a * waveguide.a) * std::cos(PI * x / waveguide.a);
		y = 0;
	}
};

struct MaskAlongX {
	double x;
	double dx;
};

double_c FindGamma1(std::vector<MaskAlongX> mask, wg::materials::Material material_0, Waveguide waveguide = waveguide_23x10) : mask_(mask), waveguide_ (waveguide) {
	
}

class PermutatorAlongX {
public:
	
	
private:
	std::vector<MaskAlongX> mask_;
	Waveguide waveguide_;
};
}
}
