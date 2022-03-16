#include "memory_pool.h"

#include <iostream>
#define DEBUG

namespace myskill {
	memory_c_pool* memory_c_pool::memory_pool = nullptr;

	memory_c_pool* memory_c_pool::GetInstance()
	{
		static DelClass del;
		if (!memory_c_pool::memory_pool) {
			memory_c_pool::memory_pool = new memory_c_pool;
		}
		return memory_c_pool::memory_pool;
	}

	void* memory_c_pool::AllocMemory(size_t size)
	{
		// 申请的是大块内存
		if (static_cast<int>(MemoryConfig::MAX_SIZE) < size) {
			void* p = new char[size];
			memset(p, '\0', size);
			manage->larger.insert(p);
			return p;
		}
		else {		// 小块内存
			// 将申请的内存块向上取整，找到对应区块
			size_t real_size = RoundUp(size);
			unsigned char index = real_size / static_cast<size_t>(MemoryConfig::ALIGN) - 1;
			p_obj p = free_list[index];
			if (p) {
				free_list[index] = p->free_list_link;
				return p;
			}
			else {
				// 当前链表中没有内存，就去内存池里找
				if (heap_size >= real_size) {
					void* p = free_start;
					free_start = free_start + real_size;
					heap_size -= real_size;
					return p;
				}

				return FillUpMemory(index, real_size);
			}
		}
		return nullptr;
	}

	void* memory_c_pool::FillUpMemory(int index, size_t size)
	{
		// 内存池中可能有剩余残留空间，但无法分配给用户,直接将这块内存存储到合适链表中去
		if (heap_size != 0) {
			unsigned char heap_index = RoundDown(heap_size) / static_cast<size_t>(MemoryConfig::ALIGN) - 1;
			p_obj p = static_cast<p_obj>(free_start);
			p->free_list_link = free_list[heap_index];
			free_list[heap_index] = p;
			heap_size = 0;
		}

		// 申请21块这类型大小的内存，10块预留给内存池，10块用来填充对应链表，还有一块分配给用户
		p_larger_memory p = new m_larger_memory(size * 21);
		if (manage->new_memory) {
			p->next = manage->new_memory;
		}
		manage->new_memory = p;
		
		// 将分配的内存放到链表中去
		void* ptr = p->p_memory + size;
		for (int i = 0; i < 10; i++) {
			p_obj m = static_cast<p_obj>(ptr);
			m->free_list_link = free_list[index];
			free_list[index] = m;
			ptr = ptr + size;
		}

		// 补充内存池
		free_start = ptr;
		free_end = p->p_memory + (size * 21);
		heap_size = size * 10;

		return p->p_memory;
	}

	void memory_c_pool::FreeMemory(void* p,size_t size)
	{
		// 归还大块内存
		if (static_cast<int>(MemoryConfig::MAX_SIZE) < size) {
			delete[] p;
			manage->larger.erase(manage->larger.find(p));
		}
		else {
			size_t real_size = RoundUp(size);
			unsigned char index = real_size / static_cast<size_t>(MemoryConfig::ALIGN) - 1;
			p_obj ptr = static_cast<p_obj>(p);

			ptr->free_list_link = free_list[index];
			free_list[index] = ptr;
		}
	}
}