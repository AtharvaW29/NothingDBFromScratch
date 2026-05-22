#include "storage/table_page.h"

#include <cstring>

namespace NothingDB{
	TablePage::TablePage() : Page() {}

	void TablePage::Init()
	{
		*GetFreeSpacePointer() = PAGE_SIZE;
		*GetTupleCountPointer() = 0;
	}

	uint16_t* TablePage::GetFreeSpacePointer() 
	{
		return reinterpret_cast<uint16_t*>(GetData());
	}
	
	uint16_t* TablePage::GetTupleCountPointer()
	{
		return reinterpret_cast<uint16_t*>(GetData() + sizeof(uint16_t));
	}

	TupleInfo* TablePage::GetTupleInfo(uint16_t slot_num)
	{
		size_t offset = sizeof(uint16_t) * 2 + slot_num * sizeof(TupleInfo);
		return reinterpret_cast<TupleInfo*>(GetData() + offset);
	}

	uint16_t TablePage::GetTupleCount() const
	{
		return *reinterpret_cast<const uint16_t*>(GetData() + sizeof(uint16_t));
	}

	bool TablePage::InsertTuple(const char* data, uint16_t size)
	{
		uint16_t free_space_pointer = *GetFreeSpacePointer();
		uint16_t tuple_count = *GetTupleCountPointer();
		uint16_t slot_directory_end = sizeof(uint16_t) * 2 + (tuple_count + 1) * sizeof(TupleInfo);
		if (free_space_pointer < slot_directory_end + size) {
			return false; // Not enough space
		}
		
		free_space_pointer -= size;
		std::memcpy(GetData() + free_space_pointer, data, size);

		TupleInfo* tuple_info = GetTupleInfo(tuple_count);

		tuple_info->offset = free_space_pointer;
		tuple_info->size = size;

		*GetFreeSpacePointer() = free_space_pointer;
		(*GetTupleCountPointer())++;

		return true;
	}

	bool TablePage::GetTuple(uint16_t slot_num, char* buffer, uint16_t& size)
	{
		if (slot_num >= GetTupleCount())
		{
			return false; // Invalid slot number
		}

		TupleInfo* tuple_info = GetTupleInfo(slot_num);

		std::memcpy(buffer, GetData() + tuple_info->offset, tuple_info->size);

		size = tuple_info->size;
		return true;
	}
}