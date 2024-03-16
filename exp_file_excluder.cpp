#include "exp_file_excluder.h"

namespace wg {

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
		throw excp_file_not_found("save file: " + file);
	}
	
	os << sp;
}
}

