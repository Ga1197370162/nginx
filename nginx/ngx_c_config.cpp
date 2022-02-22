#include "ngx_c_config.h"

namespace myskill {
	ngx_c_config* ngx_c_config::config = nullptr;
	mutex ngx_c_config::m_thread_mutex;

	ngx_c_config::ngx_c_config() {
		fstream file("nginx.conf");
		
#ifdef DEBUG
		if (!file) {

		}
		else {

		}
#endif

		string buf;
		while (!file.eof()) {
			getline(file, buf);
			if (buf[0] == '[' || buf[0] == '#' || buf[0] == ' ' || buf[0] == '\n' || buf[0] == '\t') continue;
			else PushConfig(buf);
		}

		file.close();

#ifdef DEBUG
		
#endif
	}

	void ngx_c_config::PushConfig(string& str) {
		for (string::iterator it = str.begin(); it != str.end(); it++) {
			if((*it) == ' ') str.erase(it);
		}

		string::size_type index = str.find("=");
		if (index == -1) return;

		config_list.insert(mapvalue(str.substr(0, index), str.substr(index+1,str.length()-1)));
	}

	// 线程安全，采用了双锁
	ngx_c_config* ngx_c_config::GetInstance() {
		static DeleteClass del;
		if (config == nullptr) {
			unique_lock<mutex> lock(m_thread_mutex);
			if (config == nullptr) {
				config = new ngx_c_config;
			}
		}
		return config;
	}

	const string ngx_c_config::GetConfigRow(const string& row_name) {
		maptype::iterator it = config_list.find(row_name);
		return it == config_list.end() ? nullptr : (*it).second;
	}

	const int ngx_c_config::GetConfigRowInt(const string& row_name) {
		maptype::iterator it = config_list.find(row_name);
		return it == config_list.end() ? -1 : atoi((*it).second.c_str());
	}
}