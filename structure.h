#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <optional>
#include "entities.h"
#include "materials.h"
namespace wg{
	
struct Layer{
		materials::Material material;
		double_long d;
};


class Struct {
struct excp_struct_already_adjust{};
public:
	Struct&  AddLayer(const Layer& layer) {
		st_.push_back(layer);
		return *this;
	}
	
	Struct&  AddLayer(const Layer& layer, size_t pos) {
		st_.insert(st_.begin() + pos + adjust_layer_count_, layer);
		return *this;
	}
	
	Struct& ReplaceLayer(const Layer& layer, int pos) {
		st_[pos + adjust_layer_count_] = layer;
		return *this;
	}
	
	Struct& RemoveLayer(int pos) {
		st_.erase(st_.begin() + pos + adjust_layer_count_);
		return *this;
	}
	
	const Layer& GetLayer(int pos) const {
		return st_[pos + adjust_layer_count_];
	}
	
	const std::vector<Layer>& GetAdjustStruct() const {
		return st_;
	}
	
	void Adjust(Offset offset, std::optional<wg::materials::Epsilon> dEps = std::nullopt) {
		if(dEps_) {
			throw excp_struct_already_adjust {};
		}
		
		offset_ = offset;
		/*if(adjust_layer_count_ == 1) {
			adjust_layer_ = adjust_layer;
			if(adjust_layer_) {
				ReplaceLayer(*adjust_layer_, 0);
			}
			else {
				RemoveLayer(0);
				adjust_layer_count_ = 0;
			}
		}
		else {
			adjust_layer_ = adjust_layer;
			if(adjust_layer_) {
				AddLayer(*adjust_layer_, 1);
				adjust_layer_count_ = 1;
			}
		}*/
		
		if(dEps) {
			dEps_ = dEps;
			std::for_each(st_.begin(), st_.end(), [this] (auto& layer) {
				layer.material.eps += *dEps_;
			});
		}
	}
	
	Offset GetOffset() const {
		return offset_;
	}
	
	static Struct Create(const Layer& background, const Layer& even, const Layer& odd, int count_layers){//, Offset offset = {0, 0}) {
		Struct st;
		st.AddLayer(background);
		for(int i = 0; i < count_layers; ++i) {
			st.AddLayer(i % 2 == 0 ? even : odd);
		}
		
		st.AddLayer(background);
		//st.Adjust(offset, std::nullopt);
		return st;
	}
	
private:	
	std::vector<Layer> st_;
	//std::vector<Layer> st_adjust_;
	Offset offset_;
	std::optional<wg::materials::Epsilon> dEps_;
	size_t adjust_layer_count_ = 0;
};
}
