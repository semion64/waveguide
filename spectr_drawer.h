#pragma once

#include "visual.h"
#include "waveguide.h"
#include "agilent_spectr.h"

namespace wg {
class SpectrDrawer {
public:
	SpectrDrawer(const std::string& title, ExpFileExcluder* file_excluder) : title_(title), file_excluder_(file_excluder) { }
	
	void Add(std::string title, const PhotonStructure& structure, double_long f_begin, double_long f_end, double_long f_step, Offset offset = {0, 0}) {
		spectrs_.emplace_back(title, wg::calc::BuildSpectrR(f_begin, f_end, f_step, structure, offset));
	}
	
	void Add(std::string title, const PhotonStructure& structure, f_vector fv, Offset offset = {0, 0}) {
		spectrs_.emplace_back(title, wg::calc::BuildSpectrR(fv, structure, offset));
	}
	
	void Add(std::string title, PhotonStructure&& structure, double_long f_begin, double_long f_end, double_long f_step, Offset offset = {0, 0}) {
		spectrs_.emplace_back(title, wg::calc::BuildSpectrR(f_begin, f_end, f_step, structure, offset));
	}
		
	void Add(std::string title, PhotonStructure&& structure, f_vector fv, Offset offset = {0, 0}) {
		spectrs_.emplace_back(title, wg::calc::BuildSpectrR(fv, structure, offset));
	}
	
	void Add(std::string title, std::string file) {
		spectrs_.emplace_back(title, file_excluder_->LoadSpectrR(file));
	}
	
	void Add(std::string title, DataXY data) {
		spectrs_.emplace_back(title, data);
	}
	
	void Draw() {
		std::cout << "drawing: " << title_ << std::endl;
		visual::DrawerPlot* plot = new visual::GnuplotDrawer(title_);
		plot->Start();
		for(const auto& s : spectrs_) {
			plot->AddDataSeries(s);
		}
		plot->Draw();
		plot->End();
		delete plot;
	}
	
private:
	std::string title_;
	std::vector<visual::DataSeries> spectrs_;
	ExpFileExcluder* file_excluder_;
};
}
