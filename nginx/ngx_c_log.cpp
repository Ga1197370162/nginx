#include "ngx_c_log.h"

namespace myskill {
	ngx_c_log* ngx_c_log::m_log = nullptr;
	mutex ngx_c_log::m_thread_mutex;

	ngx_c_log::ngx_c_log() {
		file.open(ngx_c_config::GetInstance()->GetConfigRow("Log"), ios::in | ios::app);
#ifdef DEBUG
		if (!file) {

		}
		else {

		}
#endif
	}

	ngx_c_log* myskill::ngx_c_log::GetInstance()
	{
		static DeleteClass del;
		if (m_log == nullptr) {
			unique_lock<mutex> lock(m_thread_mutex);
			if (m_log == nullptr) {
				m_log = new ngx_c_log;
			}
		}
		return m_log;
	}
}
