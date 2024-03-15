#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "entities.h"
#include "materials.h"

namespace wg{
	
struct Layer{
		materials::Material material;
		double_long d;
};

class PhotonStructure {
public:
	void LoadSParams(std::string file) {
		std::ifstream is(file);
		//...
	}
	
	void AddLayer(const Layer& layer) {
		structure_.push_back(layer);
	}
	
	PhotonStructure& ReplaceLayer(const Layer& layer, int pos) {
		structure_[pos] = layer;
		return *this;
	}
	
	const Layer& GetLayer(int pos) const {
		return structure_[pos];
	}
	
	const std::vector<Layer>& GetStructure() const {
		return structure_;
	}
	
	static PhotonStructure Create(Layer background, Layer even, Layer odd, int count_layers) {
		PhotonStructure structure;
		structure.AddLayer(background);
		for(int i = 0; i < count_layers; ++i) {
			structure.AddLayer(i % 2 == 0 ? even : odd);
		}
		
		structure.AddLayer(background);
		return structure;
	}
	
private:	
	std::vector<Layer> structure_;
};
}
