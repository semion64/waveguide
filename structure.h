#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "entities.h"
#include "materials.h"
#include <optional>
namespace wg{
	
struct Layer{
		materials::Material material;
		double_long d;
};

class Struct {
public:
	Struct&  AddLayer(const Layer& layer) {
		structure_.push_back(layer);
		return *this;
	}
	
	Struct&  AddLayer(const Layer& layer, size_t pos) {
		structure_.insert(structure_.begin() + pos, layer);
		return *this;
	}
	
	Struct& ReplaceLayer(const Layer& layer, int pos) {
		structure_[pos] = layer;
		return *this;
	}
	
	const Layer& GetLayer(int pos) const {
		return structure_[pos];
	}
	
	const std::vector<Layer>& GetAdjustStruct() const {
		return structure_;
	}
	
	void Adjust(Offset offset, Layer adjust_1st_layer) {
		offset_ = offset;
		if(!adjust_1st_layer_) {
			adjust_1st_layer_ = std::make_optional(adjust_1st_layer);
		}
		else {
			*adjust_1st_layer_ = adjust_1st_layer;
		}
		AddLayer(*adjust_1st_layer_, 1);
	}
	
	Offset GetOffset() const {
		return offset_;
	}
	
	static Struct Create(Layer background, Layer even, Layer odd, int count_layers, Offset offset = {0, 0}) {
		Struct structure;
		structure.AddLayer(background);
		for(int i = 0; i < count_layers; ++i) {
			structure.AddLayer(i % 2 == 0 ? even : odd);
		}
		
		structure.AddLayer(background);
		//structure.Adjust(offset, std::nullopt);
		return structure;
	}
	
private:	
	std::vector<Layer> structure_;
	Offset offset_;
	std::optional<Layer> adjust_1st_layer_;
};
}
