#pragma once
#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

//	采用单例模式
//	结构模型  小块内存（hashtable）、大块内存（链式存储）
//	通过定义不同的内存大小块分配空间

#include <iostream>
#include <vector>

#include "tool.h"

#define DEBUG

// 内存池 --- 定义在/Pool/memory_pool
namespace myskill {
	using namespace std;

	enum MemoryConfig {
		SMALL_SIZE_MIN = 8,				//	小块内存最小单元块
		SMALL_SIZE_MAX = 128,			//	小块内存最大单元块,同时也是判断大块内存的条件
		SIGAL_MEMORY_SIZE = 1024,		//	小块内存单块分配区大小

		MEMORY_LARGER_LIMIT	= 4096		//	限制最大内存块
	};

	// 小块内存结构体
	typedef struct MemorySmall {
		void* start;		//	永远指向内存空闲块头部
		void* memory;		//	堆内存
		void* end;			//	堆底部

		size_t size;		//	这块内存按这个大小分配
		size_t num;			//	这块内存剩余块数
		size_t flag;		//	标志位（0000 1111）0：未分配 1：已分配 

		MemorySmall* next;
		MemorySmall(size_t _size) noexcept :start(nullptr), memory(nullptr), size(_size), num(SIGAL_MEMORY_SIZE / _size), next(nullptr) {
			this->memory = new char[_size];
			this->start = this->memory;
			this->end = this->start + static_cast<size_t>(SIGAL_MEMORY_SIZE);
		}
		~MemorySmall() {
			delete[] start;
			start = memory = nullptr;
			size = 0;
			num = 0;
		}
	}m_memory_small, * p_memory_small;

	// 大块内存结构体
	typedef struct MemoryLarge {
		void* memory;			//	当前内存块中获取的内存
		MemoryLarge* next;		//	指向下一个大块内存
	}m_memory_large, * p_memory_large;

	// 内存总管	---	用于存储内存的相关信息
	typedef struct MemoryMaster {
		vector<p_memory_small> SmallMemory;			//	小块内存（没有元素的时候空挂）
		p_memory_large LargeMemory;					//	大块内存（采用链式存储）
		MemoryMaster() noexcept : LargeMemory(nullptr) {
			for (int i = SMALL_SIZE_MIN; i <= SMALL_SIZE_MAX; i = i << 1) {
				SmallMemory.push_back(nullptr);
			}
#ifdef DEBUG
			cout << SmallMemory.size() << endl;
#endif 
		}
		~MemoryMaster() {
			SmallMemory.clear();
		}
	}m_memory_master, * p_memory_master;

	typedef class memeory_c_pool ngx_memory_pool, * p_ngx_memory_pool;
	using p_memeory_pool = memeory_c_pool*;
	class memeory_c_pool :public noncopyable {
	private:
		static p_memeory_pool memory;		//	单例模式
		p_memory_master master;				//	内含管理内存池的指针

		memeory_c_pool() : master(new m_memory_master) {}

		// 向内存池申请内存
		void* AllocMemory(size_t m_size);

		// 小块内存分配内存出去
		void* SmallAllocMemory(p_memory_small p_memory, size_t current_larger);
	public:
		~memeory_c_pool() noexcept;			//	存在循环，不适合inline
		class DeleteThis {
		public:
			DeleteThis() {}
			~DeleteThis() {
				p_memeory_pool p = memeory_c_pool::GetInstance();
				if (p) {
					delete p;
					p = nullptr;
				}
			}
		};

		static p_memeory_pool GetInstance() noexcept;

		// 根据模板推导来获取相应类型内存
		template<typename type = char> 
		type* make_AllocMemory(size_t m_size = 1);

		
	};

	template<typename type>
	inline type* memeory_c_pool::make_AllocMemory(size_t m_size)
	{
		return static_cast<type*>(AllocMemory(sizeof(type) * m_size));
	}
}

#endif 
