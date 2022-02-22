#pragma once
#include <fstream>
#include <mutex>

#include "global.h"
#include "ngx_c_config.h"

namespace myskill {
	using namespace std;
	class ngx_c_log:public ngx_noncopyable {
	private:
		static ngx_c_log* m_log;
		static mutex m_thread_mutex;
		fstream file;

		ngx_c_log();
	public:
		class DeleteClass {
		public:
			DeleteClass() {}
			~DeleteClass() {
				unique_lock<mutex> lock(m_thread_mutex);

				ngx_c_log::GetInstance()->file.close();

				if (m_log) {
					delete m_log;
					m_log = nullptr;
				}
			}
		};

		~ngx_c_log() {}
		static ngx_c_log* GetInstance();

		template<typename T>
		fstream& operator<<(T& log_message);
		ngx_c_log& operator>>(const ngx_c_log&) = delete;
	};

	template<typename T>
	inline fstream& ngx_c_log::operator<<(T& log_message)
	{
		file << log_message;
		return file;
	}
}