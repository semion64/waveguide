
#include "visual.h"

namespace visual {
void DrawerPlot::AddDataSeries(const DataSeries& series) {
	data_series_.push_back(series);
}

void DrawerPlot::Start() {
	data_series_.clear();
}

void DrawerPlot::End() {
	data_series_.clear();
}

GnuplotDrawer::GnuplotDrawer(const std::string& title) : DrawerPlot(title) { }

void GnuplotDrawer::Draw() {
	 Gnuplot gp;
	 gp << "set title '" << title_ << "' \n";
	 //gp << "set xrange [-5:5]\nset yrange [0:25]\n";
	 bool is_first = true;
	 gp << "plot";
	 for(const auto& ds : data_series_) {
		if(!is_first) {
			gp << ", ";
		} 
		else {
			is_first = false;
		}
		gp << " '-' with lines";// title '"<< ds.title << "'";
	 }
	 
	 gp << "\n";
	 
	 for(const auto& ds : data_series_) {
		std::vector<std::vector<double>> pts;
		for(auto pt : ds.points) {
			pts.push_back(std::vector<double> {pt.x, pt.y});
		}
		gp.send1d(pts);
	 }
}
}

