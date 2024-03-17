#pragma once

#include "visual.h"
#include "waveguide.h"
#include "agilent_spectr.h"

namespace wg {
class SpectrDrawer {
public:
	SpectrDrawer(const std::string& title, const FileExcluder& file_excluder) : title_(title), file_excluder_(file_excluder) { }
	
	void Add(std::string title, const Struct& st, double_long f_begin, double_long f_end, double_long f_step) {
		spectrs_.emplace_back(title, wg::calc::BuildSpectrR(f_begin, f_end, f_step, st));
	}
	
	void Add(std::string title, const Struct& st, f_vector fv) {
		spectrs_.emplace_back(title, wg::calc::BuildSpectrR(fv, st));
	}
	
	void Add(std::string title, Struct&& st, double_long f_begin, double_long f_end, double_long f_step) {
		spectrs_.emplace_back(title, wg::calc::BuildSpectrR(f_begin, f_end, f_step, st));
	}
		
	void Add(std::string title, Struct&& st, f_vector fv) {
		spectrs_.emplace_back(title, wg::calc::BuildSpectrR(fv, st));
	}
	
	void Add(std::string title, std::string file) {
		spectrs_.emplace_back(title, file_excluder_.LoadSpectrR(file));
	}
	
	void Add(std::string title, const DataXY& data) {
		spectrs_.emplace_back(title, data);
	}
	
	void Draw() {
		std::cout << "drawing: " << title_ << std::endl;
		std::unique_ptr<visual::DrawerPlot> plot = std::make_unique<visual::GnuplotDrawer>(title_);
		plot->Start();
		for(const auto& s : spectrs_) {
			plot->AddDataSeries(s);
		}
		plot->Draw();
		plot->End();
	}
	
private:
	std::string title_;
	std::vector<visual::DataSeries> spectrs_;
	const FileExcluder& file_excluder_;
};
}
