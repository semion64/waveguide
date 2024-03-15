#include "stat_analize.h"

namespace stat_analize {

Point FindMinY(DataXY data, Offset offset) {
	assert(data.size() > 0);
	Point min = {data[0].x + offset.dx, data[0].y + offset.dy};
	for(auto pt : data) {
		if(pt.y + offset.dy < min.y) {
			min.x = pt.x + offset.dx;
			min.y = pt.y + offset.dy;
		}
	}
	
	return min;
}
}
