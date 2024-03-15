#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "entities.h"

namespace agilent {

struct exc_file_not_open {};
	
struct DataLine {
	double 	f,
			s11_real,
			s11_imag,
			s21_real,
			s21_imag,
			s12_real,
			s12_imag,
			s22_real,
			s22_imag;
};


typedef std::vector<DataLine> Spectr_s_param;

/*Spectr_s_param Load_s_param(istream& is) {
	std::string line;
	Spectr_s_param spectr;
	for(int i = 0; i < 9; ++i) { // skip input hat
		std::getline(is, line);
	}
	
	DataLine dl;
	while(is >> dl.f >> dl.s11_real >> dl.s11_imag >> dl.s21_real >> dl.s21_imag >> dl.s12_real >> dl.s12_imag >> dl.s22_real >> dl.s22_imag) {
		spectr.add(dl);
	}
	return spectr;
}*/

std::istream& operator>>(std::istream& is, Spectr_s_param& st) {
	int n = 0;
	std::string line;
	for(int i = 0; i < 9; ++i) { // skip input hat
		std::getline(is, line);
	}
	
	DataLine dl;
	while(is >> dl.f >> dl.s11_real >> dl.s11_imag >> dl.s21_real >> dl.s21_imag >> dl.s12_real >> dl.s12_imag >> dl.s22_real >> dl.s22_imag) {
		st.push_back(dl);
	}
	
	return is;
}

namespace helper {
Spectr_s_param LoadSParams(const std::string& file) {
	std::ifstream is(file);
	if(!is.is_open()) {
		throw exc_file_not_open{};
	}
	
	Spectr_s_param sp;
	is >> sp;
	return sp;
}

SpectrRT ConvertToRTSpectr(const Spectr_s_param& spectr_s) {
	SpectrRT spectr_rt;
	
	for(const auto& s : spectr_s) {
		SpectrRTValue srt {s.f, s.s11_real, s.s21_real};
		spectr_rt.push_back(srt);
	}
	return spectr_rt;
}

template <typename Spectr>
std::vector<double> get_f_vector(Spectr sp) {
	std::vector<double> f_vec;
	for(const auto& s : sp) {
		f_vec.push_back(s.f);
	}
	
	return f_vec;
}

DataXY get_graph_FR(SpectrRT sp) {
	DataXY g;
	for(const auto& s : sp) {
		g.emplace_back(s.f, s.R);
	}
	
	return g;
}
}

DataXY LoadSpectrR(const std::string& file) {
	return helper::get_graph_FR(helper::ConvertToRTSpectr(helper::LoadSParams(file)));
}

std::vector<double> LoadF(const std::string& file) {
	return helper::get_f_vector(helper::ConvertToRTSpectr(helper::LoadSParams(file)));
}

SpectrRT LoadSpectrRT(const std::string& file) {
	return helper::ConvertToRTSpectr(helper::LoadSParams(file));
}


std::ostream& operator<<(std::ostream& os, SpectrRT sp) {
	int n = 0;
	for(const auto& line: sp) {
		os << line.f << "\t" << line.R << "\t" << line.T << std::endl;
	}
	
	return os;
}

void Save(const std::string& file, const SpectrRT& sp) {
	std::ofstream os(file);
	if(!os.is_open()) {
		throw exc_file_not_open{};
	}
	
	os << sp;
}

}


