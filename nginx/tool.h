#pragma once

namespace myskill {
	typedef class noncopyable {
	private:
		noncopyable(const noncopyable&) = delete;
		noncopyable& operator=(const noncopyable&) = delete;
	public:
		noncopyable() = default;
		~noncopyable() = default;
	}ngx_noncopyable;

	// ���ü���


	// ����ָ��
	template<typename T> 
	class ngx_smart_ptr {
	private:
		T* orgin_ptr;
	public:
		ngx_smart_ptr(const T* ptr = nullptr) : orgin_ptr(const_cast<T*>(ptr)) { }
		~ngx_smart_ptr() {
			if (orgin_ptr) {
				delete orgin_ptr;
				orgin_ptr = nullptr;
			}
		}

		// �ж�ָ���Ƿ�Ϊ��
		operator bool()const {
			return this->orgin_ptr != nullptr;
		}
		
		T& operator*()const {
			return (*this->orgin_ptr);
		}
		
		T* operator->()const {
			return this->orgin_ptr;
		}
	};
}