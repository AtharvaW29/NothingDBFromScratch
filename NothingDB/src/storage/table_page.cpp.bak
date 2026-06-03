#include "storage/table_page.h"

#include <cstring>

namespace NothingDB{
	TablePage::TablePage() : Page() {}

	void TablePage::Init()
	{
		*GetFreeSpacePointer() = PAGE_SIZE;
		*GetTupleCountPointer() = 0;
		SetNextPageId(INVALID_PAGE_ID);
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
		size_t offset = sizeof(uint16_t) * 2 + sizeof(int) + slot_num * sizeof(TupleInfo);
		return reinterpret_cast<TupleInfo*>(GetData() + offset);
	}

	uint16_t TablePage::GetTupleCount() const
	{
		return *reinterpret_cast<const uint16_t*>(GetData() + sizeof(uint16_t));
	}

	bool TablePage::InsertTuple(const Tuple& tuple, RID& rid)
	{
		const auto& tuple_data = tuple.GetData();
		
		uint16_t size = static_cast<uint16_t>(tuple_data.size());
		uint16_t free_space_pointer = *GetFreeSpacePointer();
		uint16_t tuple_count = *GetTupleCountPointer();
		uint16_t slot_directory_end = sizeof(uint16_t) * 2 + sizeof(int) + (tuple_count + 1) * sizeof(TupleInfo);

		if (free_space_pointer < slot_directory_end + size) {
			return false; // Not enough space
		}

		free_space_pointer -= size;
		std::memcpy(GetData() + free_space_pointer, tuple_data.data(), size);

		TupleInfo* tuple_info = GetTupleInfo(tuple_count);

		tuple_info->offset = free_space_pointer;
		tuple_info->size = size;
		tuple_info->is_deleted = false;
		*GetFreeSpacePointer() = free_space_pointer;
		(*GetTupleCountPointer())++;
		rid = RID(GetPageId(), tuple_count);

		return true;
	}

	bool TablePage::GetTuple(uint16_t slot_num, Tuple& tuple)
	{
		if (slot_num >= GetTupleCount()) {
			return false; // Invalid slot number
		}

		TupleInfo* tuple_info = GetTupleInfo(slot_num);

		if (tuple_info->is_deleted) {
			return false;
		}

		std::vector<char> tuple_data(tuple_info->size);
		std::memcpy(
			tuple_data.data(),
			GetData() + tuple_info->offset,
			tuple_info->size
		);

		tuple.SetData(tuple_data);

		return true;
	}

	int TablePage::GetNextPageId() const {

		return *reinterpret_cast<const int*>(GetData() + sizeof(uint16_t) * 2);
	}

	void TablePage::SetNextPageId(int page_id) {
		*reinterpret_cast<int*>(GetData() + sizeof(uint16_t) * 2) = page_id;
	}

	bool TablePage::DeleteTuple(uint16_t slot_num) {
		if (slot_num >= GetTupleCount()) {
			return false; // Invalid slot number
		}
		TupleInfo* tuple_info = GetTupleInfo(slot_num);
		if (tuple_info->is_deleted) {
			return false; // Already deleted
		}
		tuple_info->is_deleted = true;
		return true;
	}

	bool TablePage::UpdateTuple(uint16_t slot_num, const Tuple& new_tuple) {
		if (slot_num >= GetTupleCount()) {
			return false;
		}
		TupleInfo* tuple_info = GetTupleInfo(slot_num);
		if (tuple_info->is_deleted) {
			return false;
		}
		const auto& new_tuple_data = new_tuple.GetData();
		uint16_t new_size = static_cast<uint16_t>(new_tuple_data.size());
		if (new_size > tuple_info->size) {
			return false; // New tuple is too large to fit in the existing space
		}
		std::memcpy(GetData() + tuple_info->offset, new_tuple_data.data(), new_size);
		tuple_info->size = new_size;
		return true;
	}
}