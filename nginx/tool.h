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

	// 引用计数


	// 智能指针
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

		// 判断指针是否为空
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