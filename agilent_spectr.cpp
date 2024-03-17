#include "agilent_spectr.h"

namespace wg{
std::istream& operator>>(std::istream& is, ExpFileExcluderAgilent::Spectr_s_param& st) {
	int n = 0;
	std::string line;
	for(int i = 0; i < 9; ++i) { // skip input hat
		std::getline(is, line);
	}
	
	ExpFileExcluderAgilent::DataLine dl;
	while(is >> dl.f >> dl.s11_real >> dl.s11_imag >> dl.s21_real >> dl.s21_imag >> dl.s12_real >> dl.s12_imag >> dl.s22_real >> dl.s22_imag) {
		st.push_back(dl);
	}
	
	return is;
}

DataXY ExpFileExcluderAgilent::LoadSpectrR(const std::string& file) {
	return get_graph_FR_(ConvertToRTSpectr_(LoadSParams_(file)));
}

DataXY ExpFileExcluderAgilent::LoadSpectrT(const std::string& file) {
	return get_graph_FT_(ConvertToRTSpectr_(LoadSParams_(file)));
}

std::vector<double> ExpFileExcluderAgilent::LoadF(const std::string& file) {
	return get_f_vector_(ConvertToRTSpectr_(LoadSParams_(file)));
}

SpectrRT ExpFileExcluderAgilent::LoadSpectrRT(const std::string& file) {
	return ConvertToRTSpectr_(LoadSParams_(file));
}

ExpFileExcluderAgilent::Spectr_s_param ExpFileExcluderAgilent::LoadSParams_(const std::string& file) {
	std::ifstream is(file);
	if(!is.is_open()) {
		throw exc_file_not_open{};
	}
	
	Spectr_s_param sp;
	is >> sp;
	return sp;
}

SpectrRT ExpFileExcluderAgilent::ConvertToRTSpectr_(const Spectr_s_param& spectr_s) {
	SpectrRT spectr_rt;
	
	for(const auto& s : spectr_s) {
		SpectrRTValue srt {s.f, s.s11_real, s.s21_real};
		spectr_rt.push_back(srt);
	}
	return spectr_rt;
}

DataXY ExpFileExcluderAgilent::get_graph_FR_(SpectrRT sp) {
	DataXY g;
	for(const auto& s : sp) {
		g.emplace_back(s.f, s.R);
	}
	
	return g;
}

DataXY ExpFileExcluderAgilent::get_graph_FT_(SpectrRT sp) {
	DataXY g;
	for(const auto& s : sp) {
		g.emplace_back(s.f, s.T);
	}
	
	return g;
}
}
