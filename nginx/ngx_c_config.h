#pragma once
// ≈‰÷√
#include <fstream>
#include <string>
#include <map>

#include "global.h"

namespace myskill {
	using namespace std;
	class ngx_c_config:public ngx_noncopyable {
	private:
		static ngx_c_config* config;
		map<string, string> config_list;

		ngx_c_config();
	public:
		class DeleteClass {
		public:
			DeleteClass() {}
			~DeleteClass() {
				delete config;
			}
		};

		~ngx_c_config() {}
		static ngx_c_config* GetInstance();
		const string GetConfigRow(const string& rowname);
		const int GetConfigRowInt(const string& rowname);
	};
}