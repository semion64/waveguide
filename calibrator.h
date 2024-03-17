#include <string>
#include "entities.h"
#include "structure.h"

namespace wg {
	
struct EpsImagParam {
		double_long EPS_IMAG_MIN = 0.0;
		double_long EPS_IMAG_MAX = 5.0;
		double_long EPS_IMAG_DELTA = 1;
		double_long EPS_IMAG_WIDTH = 0.010;
		int EPS_IMAG_N = 10;
	};
	
const EpsImagParam EPS_IMAG_PARAM_DEFAULT;
class CalibratorSpectrR {

public:
	
	CalibratorSpectrR(const FileExcluder& file_ex) : file_ex_(file_ex) { }
	
	void Calib(const Struct& st, const DataXY& spectr_exp, wg::f_vector fv, bool show_graphics = false, EpsImagParam eps_imag_param = EPS_IMAG_PARAM_DEFAULT) {
		wg::PointR min_theor = wg::calc::FindMinR(fv, st);
		wg::PointR min_exp = wg::calc::FindMinR(spectr_exp);
		
		// tune on frequence (offset on x axis)
		offset_ = {min_exp.f - min_theor.f, 0};
		
		// tune on R value (append conductivity layer in the start of structure)
		std::cout <<  eps_imag_param.EPS_IMAG_DELTA << std::endl;
		Struct st_temp = st; 
		st_temp.AddLayer(wg::Layer {wg::materials::CreateWithParams(1,eps_imag_param.EPS_IMAG_MIN), eps_imag_param.EPS_IMAG_WIDTH}, 1);
		double_long eps_imag, s;
		stat_analize::DataS<double_long> s_data;
		std::tie(eps_imag, s, s_data) =  wg::calc::BackTaskByPeackR(spectr_exp, fv, 
			eps_imag_param.EPS_IMAG_MIN, eps_imag_param.EPS_IMAG_MAX, eps_imag_param.EPS_IMAG_N, eps_imag_param.EPS_IMAG_DELTA, 
			[&st_temp, &eps_imag_param](double_long val) {
				return st_temp.ReplaceLayer(wg::Layer {wg::materials::CreateWithParams(1, val), eps_imag_param.EPS_IMAG_WIDTH}, 1);
			});
			
			layer_conductive_ = {
				wg::materials::Material {
					wg::materials::Epsilon{1, eps_imag},
					wg::materials::Mu {1, 0},
				},
				eps_imag_param.EPS_IMAG_WIDTH
			};
			
		if(show_graphics) {
			Struct st_after = st;
			st_after.Adjust(offset_, layer_conductive_);
			auto plot = wg::SpectrDrawer("Calibration offset x", file_ex_);
				plot.Add("experiment", spectr_exp);
				plot.Add("theor_{before}", st, fv);
				plot.Add("theor_{after}", st_after, fv);
			plot.Draw();
			auto plot_s = wg::SpectrDrawer("Calibration BackTask find EPS_{IMAG}", file_ex_);
				plot_s.Add("s from image", stat_analize::PointSToDataXY(s_data));
			plot_s.Draw();
		}
	}
	
	void Calib(const Struct& st, const std::string& file_name_exp, bool show_graphics = false) {
		Calib(st, file_ex_.LoadSpectrR(file_name_exp), file_ex_.LoadF(file_name_exp), show_graphics);		
	}
	
	void Adjust(Struct& st) const {
		st.Adjust(offset_, layer_conductive_);	
	}
	
private:
	Offset offset_;
	wg::Layer layer_conductive_;
	const FileExcluder& file_ex_;
	EpsImagParam eps_imag_param_;
};
}
