#include "visual.h"
#include "waveguide.h"

namespace wg {
class SpectrDrawer {
public:
	SpectrDrawer(const std::string& title) : title_(title) { }
	
	void Add(std::string title, PhotonStructure structure, double_long f_begin, double_long f_end, double_long f_step) {
		spectrs_.push_back(wg::calc::BuildSpectrR(9'200'000'000, 9'500'000'000, 1'000'000, structure.GetStructure()));
	}
	
	void Add(std::string title, PhotonStructure structure, f_vector fv) {
		spectrs_.push_back(wg::calc::BuildSpectrR(fv, structure.GetStructure()));
	}
	
	void Add(std::string title, std::string file) {
		spectrs_.push_back(wg::calc::BuildSpectrR(fv, structure.GetStructure()));
	}
	
private:
	std::string title_;
	std::vector<DataXY> spectrs_;
};
	wg::Layer air {wg::materials::CreateWithParams(1), 0};
	wg::Layer polycore {wg::materials::CreateWithParams(9.6), 0.001};
	wg::Layer fp {wg::materials::CreateWithParams(2.0), 0.009};
	
	wg::PhotonStructure ps1 = wg::PhotonStructure::Create(air, polycore, fp, 11);
	wg::Layer epox1 {wg::materials::CreateWithParams(3.0,0.0), 0.004};
	ps1.ReplaceLayer(epox1, 6);
	
	wg::PhotonStructure ps2 = wg::PhotonStructure::Create(air, polycore, fp, 11);
	wg::Layer epox2 {wg::materials::CreateWithParams(3.0,0.0), 0.0038};
	ps2.ReplaceLayer(epox2, 6);
	
	//wg::calc::CalcR(wg::calc::w(f), ps.GetStructure());
	DataXY s1 = wg::calc::BuildSpectrR(9'200'000'000, 9'500'000'000, 1'000'000, ps1.GetStructure());
	DataXY s2 = wg::calc::BuildSpectrR(9'200'000'000, 9'500'000'000, 1'000'000, ps2.GetStructure());
	
	visual::DrawerPlot* plot = new visual::GnuplotDrawer("Test Spectr");
	visual::DataSeries ds1 {"0.004 mm", s1};
	visual::DataSeries ds2 {"0.004 mm", s2};
	plot->Start();
	plot->AddDataSeries(ds1);
	plot->AddDataSeries(ds2);
	plot->Draw();
	plot->End();
	delete plot;
}
