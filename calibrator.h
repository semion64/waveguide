#include <string>
#include "entities.h"
#include "structure.h"

namespace wg {
	
struct AdjustEpsParam {
	double_long min = 0.0;
	double_long max = 0.2;
	double_long delta = 0.001;
	int N = 100;
};
	
const AdjustEpsParam ADJUST_EPS_PARAM_DEFAULT;

class CalibratorSpectrR {
public:
	
	CalibratorSpectrR(const FileExcluder& file_ex) : file_ex_(file_ex) { }
	
	void CalibF(const Struct& st, const DataXY& spectr_exp, wg::f_vector fv, bool show_graphics = false) {
		std::cout << "CalibF: " << std::endl;
		wg::PointR min_theor = wg::calc::FindMinR(fv, st);
		wg::PointR min_exp = wg::calc::FindMinR(spectr_exp);
		
		// tune on frequence (offset on x axis)
		offset_ = {min_exp.f - min_theor.f, 0};
		std::cout << "\tdelta_f = " << offset_.dx << std::endl;			
		if(show_graphics) {
			Struct st_after = st;
			st_after.Adjust(offset_, dEps_);
			auto plot = wg::SpectrDrawer("CalibF", file_ex_);
				plot.Add("experiment", spectr_exp);
				plot.Add("theor_{before}", st, fv);
				plot.Add("theor_{after}", st_after, fv);
			plot.Draw();
			
		}
	}
	
	void CalibR(const Struct& st, const DataXY& spectr_exp, wg::f_vector fv, bool show_graphics = false, AdjustEpsParam eps_imag_param = ADJUST_EPS_PARAM_DEFAULT) {		
		// tune on R value (append conductivity layer in the start of structure)
		std::cout << "CalibR: " << std::endl;
		
		double_long eps_imag, s;
		stat_analize::DataS<double_long> s_data;
		std::tie(eps_imag, s, s_data) =  wg::calc::BackTaskByPeackR(spectr_exp, fv, 
			eps_imag_param.min, eps_imag_param.max, eps_imag_param.N, eps_imag_param.delta, 
			[&st, &eps_imag_param](double_long val) {
				Struct st_temp = st;
				st_temp.Adjust({0,0}, wg::materials::Epsilon{0, val}); 
				return st_temp;
			});
		std::cout << "\tdelta_eps_imag = " << eps_imag << ", s = " << s << std::endl;	
		dEps_ = wg::materials::Epsilon{0, eps_imag};	
			
		if(show_graphics) {
			Struct st_after = st;
			st_after.Adjust(offset_, dEps_);
			auto plot = wg::SpectrDrawer("CalibR", file_ex_);
				plot.Add("experiment", spectr_exp);
				plot.Add("theor_{before}", st, fv);
				plot.Add("theor_{after}", st_after, fv);
			plot.Draw();
			auto plot_s = wg::SpectrDrawer("Calibration BackTask find EPS_{IMAG}", file_ex_);
				plot_s.Add("s from image", stat_analize::PointSToDataXY(s_data));
			plot_s.Draw();
		}
	}
	
	void CalibF(const Struct& st, const std::string& file_name_exp, bool show_graphics = false) {
		CalibF(st, file_ex_.LoadSpectrR(file_name_exp), file_ex_.LoadF(file_name_exp), show_graphics);		
	}
	
	void CalibR(const Struct& st, const std::string& file_name_exp, bool show_graphics = false) {
		CalibR(st, file_ex_.LoadSpectrR(file_name_exp), file_ex_.LoadF(file_name_exp), show_graphics);		
	}
	
	void Adjust(Struct& st) const {
		st.Adjust(offset_, dEps_);	
	}
	
private:
	Offset offset_;
	wg::materials::Epsilon dEps_;
	const FileExcluder& file_ex_;
	AdjustEpsParam adjust_eps_param_;
};
}
