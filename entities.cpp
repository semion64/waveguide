#include "entities.h"

DataXY operator-(const DataXY& l, const DataXY& r) {
	assert((l.size() == r.size()) && "DataXY substraction, arrays have different sizes");
	DataXY sub;
	for(int i = 0; i < l.size(); ++i) {
		sub.emplace_back(l[i].x, l[i].y - r[i].y);
	}
	
	return sub;
}

std::ostream& operator<<(std::ostream& os, const DataXY& r) {
	for(auto pt : r) {
		os << pt.x << "\t" << pt.y << std::endl;
	}
	
	return os;
}
