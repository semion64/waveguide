// sudo apt install libboost-all-dev
// sudo apt install gnuplot
// g++  main.cpp -lboost_iostreams -lboost_system -lboost_filesystem

#include <vector>
#include <cmath>
#include <string>
#include "entities.h"
#include "gnuplot-iostream.h"

namespace visual {

struct DataSeries {
	std::string title;
	DataXY points;
};

class DrawerPlot {
public:
	DrawerPlot (const std::string& title) {
		title_ = title;
	}
	void Start();
	void AddDataSeries(const DataSeries& series);
	virtual void Draw() = 0;
	void End();
protected:
	std::vector<DataSeries> data_series_;
	std::string title_;
}; 

class GnuplotDrawer : public DrawerPlot {
public:
	GnuplotDrawer(const std::string& title);// : DrawerPlot(name) { }
	void Draw() override;
};
}
/*
int main() {
     Gnuplot gp;
	 std::vector<std::vector<double>> pts;
	 std::vector<double> pty;
	 for(int x = -5; x <= 5; ++x) {
		pts.push_back(std::vector<double> {x, x*x});
	 }
	 gp << "set xrange [-5:5]\nset yrange [0:25]\n";
     gp << "plot '-' with lines title 'pts_A'\n";
     gp.send1d(pts);
} // very simple tool right???
*/
