#pragma once
#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

//	���õ���ģʽ
//	�ṹģ��  С���ڴ棨hashtable��������ڴ棨��ʽ�洢��
//	ͨ�����岻ͬ���ڴ��С�����ռ�

#include <iostream>
#include <vector>

#include "tool.h"

#define DEBUG

// �ڴ�� --- ������/Pool/memory_pool
namespace myskill {
	using namespace std;

	enum MemoryConfig {
		SMALL_SIZE_MIN = 8,				//	С���ڴ���С��Ԫ��
		SMALL_SIZE_MAX = 128,			//	С���ڴ����Ԫ��,ͬʱҲ���жϴ���ڴ������
		SIGAL_MEMORY_SIZE = 1024,		//	С���ڴ浥���������С

		MEMORY_LARGER_LIMIT	= 4096		//	��������ڴ��
	};

	// С���ڴ�ṹ��
	typedef struct MemorySmall {
		void* start;		//	��Զָ���ڴ���п�ͷ��
		void* memory;		//	���ڴ�
		void* end;			//	�ѵײ�

		size_t size;		//	����ڴ水�����С����
		size_t num;			//	����ڴ�ʣ�����
		size_t flag;		//	��־λ��0000 1111��0��δ���� 1���ѷ��� 

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

	// ����ڴ�ṹ��
	typedef struct MemoryLarge {
		void* memory;			//	��ǰ�ڴ���л�ȡ���ڴ�
		MemoryLarge* next;		//	ָ����һ������ڴ�
	}m_memory_large, * p_memory_large;

	// �ڴ��ܹ�	---	���ڴ洢�ڴ�������Ϣ
	typedef struct MemoryMaster {
		vector<p_memory_small> SmallMemory;			//	С���ڴ棨û��Ԫ�ص�ʱ��չң�
		p_memory_large LargeMemory;					//	����ڴ棨������ʽ�洢��
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
		static p_memeory_pool memory;		//	����ģʽ
		p_memory_master master;				//	�ں������ڴ�ص�ָ��

		memeory_c_pool() : master(new m_memory_master) {}

		// ���ڴ�������ڴ�
		void* AllocMemory(size_t m_size);

		// С���ڴ�����ڴ��ȥ
		void* SmallAllocMemory(p_memory_small p_memory, size_t current_larger);
	public:
		~memeory_c_pool() noexcept;			//	����ѭ�������ʺ�inline
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

		// ����ģ���Ƶ�����ȡ��Ӧ�����ڴ�
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
