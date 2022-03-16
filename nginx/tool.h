#pragma once
#ifndef __TOOL_H__
#define __TOOL_H__

namespace myskill {
	using namespace std;
	//	²»¿É¸´ÖÆ
	using ngx_noncopyable = class noncopyable {
	private:
		noncopyable(const noncopyable&) = delete;
		noncopyable& operator=(const noncopyable&) = delete;
	public:
		noncopyable() = default;
		~noncopyable() = default;
	};

	// C++14 make_unique
#ifdef _GLIBCXX_MEMORY
	template<typename T, typename... Ts>
	std::unique_ptr<T> make_unique(Ts&&... params)
	{
		return std::unique_ptr<T>(new T(forward<Ts>(params)...));
	}
#endif
}
#endif