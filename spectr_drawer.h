#pragma once

#include "visual.h"
#include "waveguide.h"
#include "agilent_spectr.h"

namespace wg {
class SpectrDrawer {
public:
	SpectrDrawer(const std::string& title) : title_(title) { }
	
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
		spectrs_.emplace_back(title, agilent::LoadSpectrR(file));
	}
	
	void Draw() {
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
};
}
