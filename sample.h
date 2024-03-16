#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "enities"
#include "exp_file_excluder.h"

namespace wg {
	
class SampleSet;

struct excp_litera_exists : public excp {
	excp_litera_exists() {}
	excp_litera_exists(std::string text) : excp(text) { }
};

class Sample {
typedef std::string Litera;
typedef std::string Name;
typedef double_long Value;

public:
	Sample(Litera litera, std::string file_name) : litera_(litera), file_name_(file_name) {}
	Litera GetLitera() {
		return litera_;
	}
	std::string AddParam(Name name, Value value) {
		params_[name] = value;
	}
	std::string GetFileName() {
		return file_name_;
	}
private:
	Litera litera_;
	std::string file_name_;
	std::map<Name, Value> params_;
	//SampleSet set;
};

class SampleSet {
public:
	SampleSet(std::string sample_set_file) {
		std::istream is(sample_set_file);
		if(!is.is_open()){
			throw excp_file_not_found{};
		}
		std::string str;
		is >> str;
		if(str != "sample_set_directory:") {
			throw excp_incorrect_file_format {"sample file " + sample_set_file + " must beginig with 'sample_set_directory: '"};
		}
		is >> files_directory_;
		char param_line[1024];
		is.getline(param_line, 1024);
		std::stringstream ss(param_line);
		
		while(!ss.eof()) {
			ss >> str;
			param_names_.push_back(str);
		}
		
		while(!is.eof()) {
			std::string litera, file_name;
			is >> litera >> file_name;
			Sample sample(litera, file_name);
			for(const auto& name : param_names_) {
				Sample::Value value;
				is >> value;
				sample.AddParam(name, value);
			}
			
			if(samples_.count(litera)) {
				throw excp_litera_exists { "litera '" + litera + "' in '" + sample_set_file + "' already exists" };
			}
			
			samples_[litera] = sample;
		}
	}
	
	void AddSample(Sampl sample) {
		samples_[sample.litera_] = sample;
	}
private:
	//ExpFileExcluder file_excluder_;
	std::string files_directory_;
	std::vector<std::string> param_names_;
	std::map<Litera, Sampl> samples_;
};
}
