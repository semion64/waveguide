#pragma once 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "entities.h"
#include "exp_file_excluder.h"

namespace wg {
struct exc_file_not_open {};
class ExpFileExcluderAgilent : public ExpFileExcluder {
public:	
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
	DataXY LoadSpectrR(const std::string& file) override;
	DataXY LoadSpectrT(const std::string& file) override;
	std::vector<double> LoadF(const std::string& file) override;
	SpectrRT LoadSpectrRT(const std::string& file) override;
	
	template <typename Spectr>
	std::vector<double> get_f_vector_(Spectr sp) {
		std::vector<double> f_vec;
		for(const auto& s : sp) {
			f_vec.push_back(s.f);
		}
		
		return f_vec;
	}
private:
	Spectr_s_param LoadSParams_(const std::string& file);
	SpectrRT ConvertToRTSpectr_(const Spectr_s_param& spectr_s);
	DataXY get_graph_FR_(SpectrRT sp);
	DataXY get_graph_FT_(SpectrRT sp);
};



}


