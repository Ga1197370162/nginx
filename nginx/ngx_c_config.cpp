#include "ngx_c_config.h"
#include <iostream>

namespace myskill {
	ngx_c_config* ngx_c_config::config = nullptr;

	ngx_c_config::ngx_c_config() {
		fstream file("nginx.conf");
		
		string buf;
		while (!file.eof()) {
			getline(file, buf);
			cout << buf << endl;
		}
	}

	ngx_c_config* ngx_c_config::GetInstance() {
		static DeleteClass del;
		if (!config) {
			config = new ngx_c_config;
		}
		return config;
	}

	const string ngx_c_config::GetConfigRow(const string& rowname) {
		
	}

	const int GetConfigRowInt(const string& rowname) {

	}
}