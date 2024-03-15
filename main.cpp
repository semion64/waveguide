#include <iostream>
#include <complex>
#include <cassert>
#include <fstream>

#include "entities.h"
#include "structure.h"
#include "waveguide.h"
#include "agilent_spectr.h"
#include "visual.h"
#include "data/task_composite.h"

std::ostream& operator<<(std::ostream& os, wg::PhotonStructure st) {
	int n = 0;
	for(const auto& layer: st.GetStructure()) {
		os << n++ << ".\t" << "d: " << layer.d  << ",\teps: " << layer.material.eps.real << ",\tsigma: " << layer.material.eps.image << ",\tmu_real: " << layer.material.mu.real << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, wg::PointR pt) {
	int n = 0;
	os << "(" << pt.R << ", " << pt.f << ")";
	return os;
}

void TestEntities() {
	double_long f = 9'350'000'000;
	std::cout.precision(30);
	std::cout << wg::PI << std::endl;
	std::cout << wg::EPS_0 << std::endl;
	std::cout << wg::MU_0 << std::endl;
	std::cout << std::sqrt(double_c(-96524.411200930)) << std::endl;
	
	wg::Layer epox {wg::materials::CreateWithParams(3.0,0.0), 0.004};
	std::cout << wg::calc::gamma(wg::calc::w(f), epox.material) << std::endl;
}

void TestSpectr() {
	wg::Layer air {wg::materials::CreateWithParams(1), 0};
	wg::Layer polycore {wg::materials::CreateWithParams(9.6), 0.001};
	wg::Layer fp {wg::materials::CreateWithParams(2.0), 0.009};
	wg::Layer epox {wg::materials::CreateWithParams(3.0,0.0), 0.004};
	wg::PhotonStructure ps = wg::PhotonStructure::Create(air, polycore, fp, 11);
	ps.ReplaceLayer(epox, 6);
	
	//wg::calc::CalcR(wg::calc::w(f), ps.GetStructure());
	wg::calc::BuildSpectrR(std::cout, 9'200'000'000, 9'500'000'000, 1'000'000, ps);		
}

void TestMin() {
	double_long e = 0.001; 
	wg::Layer air {wg::materials::CreateWithParams(1), 0};
	wg::Layer polycore {wg::materials::CreateWithParams(9.6), 0.001};
	wg::Layer fp {wg::materials::CreateWithParams(2.0), 0.009};
	wg::Layer epox {wg::materials::CreateWithParams(3.0,0.0), 0.004};
	wg::PhotonStructure ps = wg::PhotonStructure::Create(air, polycore, fp, 11);
	ps.ReplaceLayer(epox, 6);
	
	wg::PointR min = wg::calc::FindMinR(9'200'000'000, 9'500'000'000, 1'000'000, ps); 
	std::cout << "TestMin: " << min << std::endl;	
	assert((min.R > -115.108 - e && min.R < -115.108 + e) && "TestMin Wrong value");
	assert((min.f == 9.353e+09) && "TestMin Wrong f");
	
}

void TestSpectr_f_from_file() {
	wg::Layer air {wg::materials::CreateWithParams(1), 0};
	wg::Layer polycore {wg::materials::CreateWithParams(9.6), 0.001};
	wg::Layer fp {wg::materials::CreateWithParams(2.0), 0.009};
	wg::Layer epox {wg::materials::CreateWithParams(3.0,0.0), 0.004};
	wg::PhotonStructure ps = wg::PhotonStructure::Create(air, polycore, fp, 11);
	ps.ReplaceLayer(epox, 6);
	wg::f_vector_load fv = agilent::LoadF("data/c0_1.s2p");
	wg::calc::BuildSpectrR(std::cout, fv, ps);		
}

void TestLoadSpectr() {
	SpectrRT c0_1 = agilent::LoadSpectrRT("data/c0_1.s2p");
	//std::ofstream os("s");
	//os << c0_1;
	agilent::Save("s", c0_1);
}

void TestVisualSpectr() {
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
	DataXY s1 = wg::calc::BuildSpectrR(9'200'000'000, 9'500'000'000, 1'000'000, ps1);
	DataXY s2 = wg::calc::BuildSpectrR(9'200'000'000, 9'500'000'000, 1'000'000, ps2);
	
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

int main() {
	task_composite::Analize();
	//TestLoadSpectr();
	//TestSpectr();
	//TestMin();
	//TestSpectr_f_from_file();
	//TestVisualSpectr();
}
