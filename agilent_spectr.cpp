#include "agilent_spectr.h"

namespace wg{
std::istream& operator>>(std::istream& is, FileExcluderAgilent::Spectr_s_param& st) {
	int n = 0;
	std::string line;
	for(int i = 0; i < 9; ++i) { // skip input hat
		std::getline(is, line);
	}
	
	FileExcluderAgilent::DataLine dl;
	while(is >> dl.f >> dl.s11_real >> dl.s11_imag >> dl.s21_real >> dl.s21_imag >> dl.s12_real >> dl.s12_imag >> dl.s22_real >> dl.s22_imag) {
		st.push_back(dl);
	}
	
	return is;
}

DataXY FileExcluderAgilent::LoadSpectrR(const std::string& file) const {
	
	return get_graph_FR_(ConvertToRTSpectr_(LoadSParams_(file)));
}

DataXY FileExcluderAgilent::LoadSpectrT(const std::string& file) const {
	return get_graph_FT_(ConvertToRTSpectr_(LoadSParams_(file)));
}

std::vector<double> FileExcluderAgilent::LoadF(const std::string& file) const {
	return get_f_vector_(ConvertToRTSpectr_(LoadSParams_(file)));
}

SpectrRT FileExcluderAgilent::LoadSpectrRT(const std::string& file) const {
	return ConvertToRTSpectr_(LoadSParams_(file));
}

FileExcluderAgilent::Spectr_s_param FileExcluderAgilent::LoadSParams_(const std::string& file) const {
	std::ifstream is(file);
	if(!is.is_open()) {
		throw exc_file_not_open{};
	}
	
	Spectr_s_param sp;
	is >> sp;
	is.close();
	return sp;
}

SpectrRT FileExcluderAgilent::ConvertToRTSpectr_(const Spectr_s_param& spectr_s) const {
	SpectrRT spectr_rt;
	
	for(const auto& s : spectr_s) {
		SpectrRTValue srt {s.f, s.s11_real, s.s21_real};
		spectr_rt.push_back(srt);
	}
	return spectr_rt;
}

DataXY FileExcluderAgilent::get_graph_FR_(SpectrRT sp) const {
	DataXY g;
	for(const auto& s : sp) {
		g.emplace_back(s.f, s.R);
	}
	
	return g;
}

DataXY FileExcluderAgilent::get_graph_FT_(SpectrRT sp) const {
	DataXY g;
	for(const auto& s : sp) {
		g.emplace_back(s.f, s.T);
	}
	
	return g;
}
}
