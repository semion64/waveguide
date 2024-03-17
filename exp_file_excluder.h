#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "entities.h"

namespace wg {

std::ostream& operator<<(std::ostream& os, SpectrRT sp);

void Save(const std::string& file, const SpectrRT& sp);

class ExpFileExcluder {
public:
	virtual DataXY LoadSpectrR(const std::string& file) = 0;
	virtual DataXY LoadSpectrT(const std::string& file) = 0;
	virtual std::vector<double> LoadF(const std::string& file) = 0;
	virtual SpectrRT LoadSpectrRT(const std::string& file) = 0;
};
}
