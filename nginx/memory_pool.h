#pragma once
#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include <string.h>
#include <cstddef>
#include <unordered_set>
#include <mutex>

#include "tool.h"

// MemoryDivider 接口封装
// MemoryDeep --- 一级内存分配器（链表结构，对new/delete的直接封装，再将内存链接起来退出释放,主要记录向new申请的内存块，后期自动delete回收）
// MemoryPool --- 二级内存分配器（相当于hashmap，每个空间存放指定大小内存，内存池内存通过一级内存分配器获取，小块内存通过hash表上取出相应链表，大块内存直接获取）

#define DEBUG

namespace myskill {
	using  namespace std;

	class memory_c_pool;

	// 记录每一块使用new申请的内存块
	typedef struct larger_memory_c {
		void* p_memory;					//	内存块
		larger_memory_c* next;			//	指向下一块内存
		larger_memory_c(size_t size):p_memory(new char[size]),next(nullptr){
			memset(this->p_memory,'\0',size);
		}
		~larger_memory_c() {
			if (p_memory) {
				delete[] p_memory;
				p_memory = nullptr;
			}
		}
	}m_larger_memory,*p_larger_memory;

	// 内存管理器
	typedef struct memory_manage_c {
		p_larger_memory new_memory;		//	一级分配器内存
		unordered_set<void*> larger;	//	记录大块内存，用户未释放的内存，后期它来释放
		memory_manage_c():new_memory(nullptr) {}
		~memory_manage_c() {
			p_larger_memory next;
			while (new_memory && new_memory->next) {
				next = new_memory->next;
				new_memory->next = new_memory->next->next;
				delete next;
				next = nullptr;
			}
			delete new_memory;
			new_memory = nullptr;

			for (auto p_larger = larger.begin(); p_larger != larger.end(); p_larger++) {
				delete[](*p_larger);
			}
		}
	}m_memory_manage,*p_memory_manage;

	// 小块内存链表
	typedef union obj {
		union obj* free_list_link;			//	当不用该内存的时候，它可以作为链表进行存储
		char client_date[1];				//	可变数组，使用该内存的时候它就是数据
	}m_obj, * p_obj;

	class memory_c_pool:public noncopyable {
		enum class MemoryConfig {
			ALIGN = 8,
			MAX_SIZE = 128,
			FREE_LIST_NUMS = MAX_SIZE / ALIGN
		};
	private:
		static memory_c_pool* memory_pool;		//	唯一实例

		void* free_start;						//	当前内存池使用的内存起始
		void* free_end;							//	当前内存池使用的内存起始
		size_t heap_size;						//	当前内存池使用的内存剩余大小

		p_obj free_list[static_cast<int>(MemoryConfig::FREE_LIST_NUMS)];			//	以8为倍数的小块内存链表

		p_memory_manage manage;					//	内存管理器
		memory_c_pool() :manage(new m_memory_manage), free_list({ nullptr }) {}

		//	向上取整
		size_t RoundUp(size_t size) {
			size_t _ALIGN = static_cast<int>(MemoryConfig::ALIGN);
			return ((size + _ALIGN - 1) & ~(_ALIGN - 1));
		}

		//	向下取整
		size_t RoundDown(size_t size) {
			size_t _ALIGN = static_cast<int>(MemoryConfig::ALIGN);
			return ((size) & ~(_ALIGN - 1));
		}

		// 根据链块大小填充内存，并返回内存给用户
		void* FillUpMemory(int index,size_t size);
	public:
		class DelClass {
		public:
			DelClass() {}
			~DelClass() {
				if (memory_c_pool::memory_pool) {
					delete memory_c_pool::memory_pool;
					memory_c_pool::memory_pool = nullptr;
				}
			}
		};
		~memory_c_pool(){
			if (manage) {
				delete manage;
				manage = nullptr;
			}
		}
		static memory_c_pool* GetInstance();

		void* AllocMemory(size_t size);

		void FreeMemory(void* p,size_t size);
	};

	static std::mutex thread_mutex;

	//	外部接口，用于只需通过该接口获取内存
	template<typename T>
	T* make_memory(size_t size = 1) {
		std::lock_guard<mutex> lock(thread_mutex);
		size_t real_size = size * sizeof(T);
		T* p = static_cast<T*>(memory_c_pool::GetInstance()->AllocMemory(real_size));
		memset(p, '\0', real_size);
		return p;
	}

	//	归还内存，以便下次使用
	template<typename T>
	void free_memory(T* p, size_t size = 1) {
		std::lock_guard<mutex> lock(thread_mutex);
		memory_c_pool::GetInstance()->FreeMemory(static_cast<void*>(p), size * sizeof(T));
	}
}

#endif 
