#pragma once
// ≈‰÷√Œƒº˛
#include <map>

#include "tool.h"

namespace myskill {
	using namespace std;
	
	class ngx_c_config: public ngx_noncopyable {
		typedef map<const string, string> maptype;
		typedef pair<const string, string> mapvalue;
	private:
		static ngx_c_config* config;
		map<const string, string> config_list;

		ngx_c_config();
		void PushConfig(string& str);
	public:
		class DeleteClass {
		public:
			DeleteClass() {}
			~DeleteClass() {
				if (config) {
					delete config;
					config = nullptr;
				}
			}
		};

		~ngx_c_config() {}
		static ngx_c_config* GetInstance();
		const string GetConfigRow(const string& row_name);
		const int GetConfigRowInt(const string& row_name);
	};
}