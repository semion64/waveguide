#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <optional>
#include <memory>

#include "entities.h"
#include "exp_file_excluder.h"

namespace wg {

struct excp_litera_exists : public excp {
	excp_litera_exists() {}
	excp_litera_exists(std::string text) : excp(text) { }
};

struct excp_litera_not_exists : public excp {
	excp_litera_not_exists() {}
	excp_litera_not_exists(std::string text) : excp(text) { }
};

class Sample {
public:
	typedef std::string Litera;
	typedef std::string Name;
	typedef double_long Value;
	
	Sample(Litera litera, std::string file_path, const FileExcluder& file_excluder) : litera_(litera), file_path_(file_path), file_excluder_(file_excluder) {}
	Litera GetLitera() const {
		return litera_;
	}
	
	void AddParam(Name name, Value value) {
		params_[name] = value;
	}
	
	Value GetParamValue(Name name) {
		return params_[name];
	}
	
	std::string GetFilePath() const {
		return file_path_;
	}
	
	const DataXY& GetSpectrR() {
		if(!spectrR_) {
			spectrR_ = std::make_optional(std::move(file_excluder_.LoadSpectrR(file_path_)));
		}
		
		return (*spectrR_);
	}
	
	const DataXY& GetSpectrT() {
		if(!spectrT_) {
			spectrR_ = std::make_optional(std::move(file_excluder_.LoadSpectrT(file_path_)));
		}
		
		return (*spectrT_);
	}
	
	
private:
	const FileExcluder& file_excluder_;
	std::optional<DataXY> spectrR_;
	std::optional<DataXY> spectrT_;
	Litera litera_;
	std::string file_path_;
	std::map<Name, Value> params_;
};

class SampleSet {
public:
	SampleSet(const std::string& sample_set_file, const std::string& data_file_ext, const FileExcluder& exp_file_excluder) : file_excluder_(exp_file_excluder) {
		std::ifstream is(sample_set_file);
		if(!is.is_open()){
			throw excp_file_not_found{};
		}
		
		std::string str;
		is >> str;
		if(str != "directory:") {
			throw excp_incorrect_file_format {"sample file " + sample_set_file + " must beginig with 'directory: '"};
		}
		is >> files_directory_;
		
		is >> str;
		if(str != "param_init:") {
			throw excp_incorrect_file_format {"sample file " + sample_set_file + " must have second line begining with 'param_init: '"};
		}
		
		while(str != "param_init_end.") {
			is >> str;
			if(str == "param_init_end.") {
				break;
			}
			
			param_names_.push_back(str);
		}
		
		while(!is.eof()) {
			std::string litera, file_name;
			is >> litera;
			std::shared_ptr<Sample> sample = std::make_shared<Sample>(litera, files_directory_ + "/" + litera + "." + data_file_ext, file_excluder_);
			//std::cout << "============" << litera << std::endl;
			for(const auto& name : param_names_) {
				Sample::Value value;
				is >> value;
				sample->AddParam(name, value);
				//std::cout << name << ": " << value << std::endl;
			}
			
			AddSample(sample);
		}
		
		is.close();
	}
	
	void AddSample(std::shared_ptr<Sample> sample) {
		const auto& litera = sample->GetLitera();
		if(samples_.count(litera)) {
			throw excp_litera_exists ("litera '" + litera + "' already exists");
		}
		
		samples_.insert({litera, sample});
	}
	
	template<typename FuncAct, class T>
	void Action(FuncAct func_act, T arg)
	{
		func_act(arg);
	}
	
	template<typename FuncAct, class T, class... R>
	void Action(FuncAct func_act, T arg, R... rest)
	{
		func_act(arg);
		// If "rest" only has one argument, it will call the above function
		// Otherwise, it will call this function again, with the first argument
		// from "rest" becoming "arg"
		Action(rest...); 
	}
	
	template<typename SampleGetDataFunc>
	void Draw(std::string name, SampleGetDataFunc sample_get_data_func, std::initializer_list<std::string> literas) const {
		auto plot = wg::SpectrDrawer(name, file_excluder_);
		for(auto&& litera : literas) {
			plot.Add(litera, sample_get_data_func(samples_.at(litera)));
		}
		
		plot.Draw();
	}
	
	void DrawSpectrR(std::string name, std::initializer_list<std::string> literas) const{
		Draw(name, [](const auto& sample) {return sample->GetSpectrR(); }, literas);
	}
	
	PointR FindMinR(std::string name) {
		return wg::calc::FindMinR(GetSample(name)->GetSpectrR());
	}
	
	template <typename FuncY>
	DataXY ParametrDependence(std::string param, FuncY func_y, std::initializer_list<std::string> literas = {}) {
		std::vector<std::shared_ptr<Sample>> choose_samples_;
		if(literas.size() == 0) {
			for(const auto& [litera, sample] : samples_) {
				 choose_samples_.push_back(sample);
			}
		}
		else {
			for(const auto& litera : literas) {
				choose_samples_.push_back(samples_.at(litera));
			}
		}
		
		DataXY data;
		for(const auto& sample : choose_samples_) {
			data.emplace_back(sample->GetParamValue(param), func_y(sample));
		}
		
		std::sort(data.begin(), data.end(), [](const auto& l, const auto& r) {return l.x < r.x; });
		
		return data;
	}
	
	std::shared_ptr<Sample> GetSample(std::string litera) {
		if(!samples_.count(litera)) {
			throw excp_litera_not_exists{};
		}
		return samples_.at(litera);
	}
	
private:
	const FileExcluder& file_excluder_;
	std::string files_directory_;
	std::vector<std::string> param_names_;
	std::map<Sample::Litera, std::shared_ptr<Sample>> samples_;
};
}
