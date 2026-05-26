#include "storage/table_heap.h"

namespace NothingDB {
	TableHeap::TableHeap(
		BufferPoolManager* buffer_pool_manager) : buffer_pool_manager_
		(buffer_pool_manager), first_page_id_(INVALID_PAGE_ID) {
	}

	bool TableHeap::InsertTuple(const Tuple& tuple, RID& rid) {
		if (first_page_id_ == INVALID_PAGE_ID)
		{
			int new_page_id;
			Page* page = buffer_pool_manager_->NewPage(&new_page_id);
			if (page == nullptr)
			{
				return false; // Failed to allocate new page
			}
			auto* table_page = reinterpret_cast<TablePage*>(page);
			table_page->Init();

			first_page_id_ = new_page_id;
		}

		int current_page_id = first_page_id_;
		while (true)
		{
			Page* page = buffer_pool_manager_->FetchPage(current_page_id);
			if (page == nullptr)
			{
				return false; // Failed to fetch page
			}
			auto* table_page = reinterpret_cast<TablePage*>(page);
			if (table_page->InsertTuple(tuple, rid))
			{
				buffer_pool_manager_->UnpinPage(current_page_id, true);
				return true; // Successfully inserted
			}

			int next_page_id = table_page->GetNextPageId();
			if (next_page_id == INVALID_PAGE_ID)
			{
				int new_page_id;
				Page* new_page = buffer_pool_manager_->NewPage(&new_page_id);
				if (new_page == nullptr)
				{
					buffer_pool_manager_->UnpinPage(current_page_id, false);
					return false; // Failed to allocate new page
				}
				auto* new_table_page = reinterpret_cast<TablePage*>(new_page);
				new_table_page->Init();
				table_page->SetNextPageId(new_page_id);
				buffer_pool_manager_->UnpinPage(current_page_id, true);
				current_page_id = new_page_id;
				continue; // Move to the new page

			}
			else
			{
				buffer_pool_manager_->UnpinPage(current_page_id, false);
				current_page_id = next_page_id; // Move to the next page
			}
		}
	}

	std::vector <Tuple> TableHeap::Scan() {
		std::vector<Tuple> tuples;
		int current_page_id = first_page_id_;
		while (current_page_id != INVALID_PAGE_ID)
		{
			Page* page = buffer_pool_manager_->FetchPage(current_page_id);
			if (page == nullptr)
			{
				break; // Failed to fetch page, stop scanning
			}
			auto* table_page = reinterpret_cast<TablePage*>(page);
			uint16_t tuple_count = table_page->GetTupleCount();
			for (uint16_t i = 0; i < tuple_count; ++i)
			{
				Tuple tuple;
				if (table_page->GetTuple(i, tuple))
				{
					tuples.push_back(tuple);
				}
			}
			int next_page_id = table_page->GetNextPageId();
			buffer_pool_manager_->UnpinPage(current_page_id, false);
			current_page_id = next_page_id; // Move to the next page
		}
		return tuples;
	}

	bool TableHeap::GetTuple(const RID& rid, Tuple& tuple) {
		Page* page = buffer_pool_manager_->FetchPage(rid.GetPageId());
		if (page == nullptr)
		{
			return false;
		}
		auto* table_page = reinterpret_cast<TablePage*>(page);
		bool result = table_page->GetTuple(rid.GetSlotNum(), tuple);
		buffer_pool_manager_->UnpinPage(rid.GetPageId(), false);
		return result;
	}

	bool TableHeap::DeleteTuple(const RID& rid) {
		Page* page = buffer_pool_manager_->FetchPage(rid.GetPageId());
		if (page == nullptr)
		{
			return false;
		}
		auto* table_page = reinterpret_cast<TablePage*>(page);
		bool result = table_page->DeleteTuple(rid.GetSlotNum());
		buffer_pool_manager_->UnpinPage(rid.GetPageId(), result);
		return result;
	}

	bool TableHeap::UpdateTuple(const RID& rid, const Tuple& tuple) {
		Page* page = buffer_pool_manager_->FetchPage(rid.GetPageId());
		if (page == nullptr)
		{
			return false;
		}
		auto* table_page = reinterpret_cast<TablePage*>(page);
		bool result = table_page->UpdateTuple(rid.GetSlotNum(), tuple);
		buffer_pool_manager_->UnpinPage(rid.GetPageId(), result);
		return result;
	}
}