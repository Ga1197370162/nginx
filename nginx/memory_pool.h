#pragma once
#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include <string.h>
#include <cstddef>
#include <unordered_set>
#include <mutex>

#include "tool.h"

// MemoryDivider �ӿڷ�װ
// MemoryDeep --- һ���ڴ������������ṹ����new/delete��ֱ�ӷ�װ���ٽ��ڴ����������˳��ͷ�,��Ҫ��¼��new������ڴ�飬�����Զ�delete���գ�
// MemoryPool --- �����ڴ���������൱��hashmap��ÿ���ռ���ָ����С�ڴ棬�ڴ���ڴ�ͨ��һ���ڴ��������ȡ��С���ڴ�ͨ��hash����ȡ����Ӧ��������ڴ�ֱ�ӻ�ȡ��

#define DEBUG

namespace myskill {
	using  namespace std;

	class memory_c_pool;

	// ��¼ÿһ��ʹ��new������ڴ��
	typedef struct larger_memory_c {
		void* p_memory;					//	�ڴ��
		larger_memory_c* next;			//	ָ����һ���ڴ�
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

	// �ڴ������
	typedef struct memory_manage_c {
		p_larger_memory new_memory;		//	һ���������ڴ�
		unordered_set<void*> larger;	//	��¼����ڴ棬�û�δ�ͷŵ��ڴ棬���������ͷ�
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

	// С���ڴ�����
	typedef union obj {
		union obj* free_list_link;			//	�����ø��ڴ��ʱ����������Ϊ������д洢
		char client_date[1];				//	�ɱ����飬ʹ�ø��ڴ��ʱ������������
	}m_obj, * p_obj;

	class memory_c_pool:public noncopyable {
		enum class MemoryConfig {
			ALIGN = 8,
			MAX_SIZE = 128,
			FREE_LIST_NUMS = MAX_SIZE / ALIGN
		};
	private:
		static memory_c_pool* memory_pool;		//	Ψһʵ��

		void* free_start;						//	��ǰ�ڴ��ʹ�õ��ڴ���ʼ
		void* free_end;							//	��ǰ�ڴ��ʹ�õ��ڴ���ʼ
		size_t heap_size;						//	��ǰ�ڴ��ʹ�õ��ڴ�ʣ���С

		p_obj free_list[static_cast<int>(MemoryConfig::FREE_LIST_NUMS)];			//	��8Ϊ������С���ڴ�����

		p_memory_manage manage;					//	�ڴ������
		memory_c_pool() :manage(new m_memory_manage), free_list({ nullptr }) {}

		//	����ȡ��
		size_t RoundUp(size_t size) {
			size_t _ALIGN = static_cast<int>(MemoryConfig::ALIGN);
			return ((size + _ALIGN - 1) & ~(_ALIGN - 1));
		}

		//	����ȡ��
		size_t RoundDown(size_t size) {
			size_t _ALIGN = static_cast<int>(MemoryConfig::ALIGN);
			return ((size) & ~(_ALIGN - 1));
		}

		// ���������С����ڴ棬�������ڴ���û�
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

	//	�ⲿ�ӿڣ�����ֻ��ͨ���ýӿڻ�ȡ�ڴ�
	template<typename T>
	T* make_memory(size_t size = 1) {
		//std::lock_guard<mutex> lock(thread_mutex);
		size_t real_size = size * sizeof(T);
		T* p = static_cast<T*>(memory_c_pool::GetInstance()->AllocMemory(real_size));
		memset(p, '\0', real_size);
		return p;
	}

	//	�黹�ڴ棬�Ա��´�ʹ��
	template<typename T>
	void free_memory(T* p, size_t size = 1) {
		//std::lock_guard<mutex> lock(thread_mutex);
		memory_c_pool::GetInstance()->FreeMemory(static_cast<void*>(p), size * sizeof(T));
	}
}

#endif 
