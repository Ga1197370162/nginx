#include "memory_pool.h"

#include <string.h>

#define DEBUG

namespace myskill {
	p_memeory_pool memeory_c_pool::memory = nullptr;

	p_memeory_pool memeory_c_pool::GetInstance() noexcept {
		static DeleteThis del;
		if (memory == nullptr) {
			memory = new memeory_c_pool();
		}
		return memory;
	}

	memeory_c_pool::~memeory_c_pool() noexcept {
		for (auto small = this->master->SmallMemory.begin(); small != this->master->SmallMemory.end(); small += 1) {
			if (*small != nullptr) {
				delete* small;
				*small = nullptr;
			}
		}

		for (auto larger = this->master->LargeMemory; larger && larger->next; larger = larger->next) {
			auto temp = larger->next;
			larger->next = temp->next;
			delete[] temp->memory;
			temp->memory = nullptr;
			delete temp;
			temp = nullptr;
		}

		if (this->master->LargeMemory) {
			delete this->master->LargeMemory->memory;
			this->master->LargeMemory->memory = nullptr;
			delete this->master->LargeMemory;
			this->master->LargeMemory = nullptr;
		}

		delete master;
		master = nullptr;
	}

	void* memeory_c_pool::AllocMemory(size_t m_size) {
		// ������Ǵ���ڴ�
		m_size = m_size <= 0 ? 1 : m_size > MEMORY_LARGER_LIMIT ? MEMORY_LARGER_LIMIT : m_size;		//	�߽�ֵ�趨

		if (m_size > SMALL_SIZE_MAX) {
			auto p = this->master->LargeMemory;
			for (; p && p->next; p = p->next);		//	���ƶ������
			if (p) {		//	�Ѿ����ڴ���ڴ�
				p->next = new m_memory_large;
				p->next->memory = new char[m_size];
				memset(p->next->memory, '\0', m_size);
				return p->next->memory;
			}
			else {			//	û�д洢����ڴ�
				this->master->LargeMemory = new m_memory_large;		//	Ϊ���쳣��ȫ��������ڴ����Ҳû�£�����������Զ�����delete
				this->master->LargeMemory->memory = new char[m_size];
				memset(this->master->LargeMemory->memory,'\0', m_size);
				return this->master->LargeMemory->memory;
			}
		} else {		//	�������ľ���С�ڴ��
			auto p = this->master->SmallMemory;
			size_t current_larger = SMALL_SIZE_MIN;					//	��¼��ǰ��������Ǻ��ַ����С
			size_t index = 0;										//	��ʾ�����С���ڴ��Ӧ�Ĵ洢����
			for (; current_larger < m_size; current_larger = current_larger << 1 , index += 1) {}
			p_memory_small p_memory = p[index];
			if (p_memory == nullptr) {								//	��ǰ����ڴ滹�ǿ���״̬
				p_memory = new m_memory_small(current_larger);
				return SmallAllocMemory(p_memory, current_larger);
			}
			else {		//	�Ѿ���ʼ���ڴ�

			}
		}
	}

	void* memeory_c_pool::SmallAllocMemory(p_memory_small p_memory,size_t current_larger) {
		
	}
}