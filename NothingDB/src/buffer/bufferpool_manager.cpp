#include "buffer/bufferpool_manager.h"

namespace NothingDB{

	BufferPoolManager::BufferPoolManager(
		size_t pool_size, DiskManager* disk_manager)
		: pool_size_(pool_size), 
		disk_manager_(disk_manager), 
		replacer_(pool_size) {
		pages_ = new Page[pool_size_];
		for (size_t i = 0; i < pool_size_; i++) {
			free_list_.push_back(static_cast<int>(i));
		}
	}

	BufferPoolManager::~BufferPoolManager() {
		for (size_t i = 0; i < pool_size_; i++) {
			if (pages_[i].IsDirty()) {
				disk_manager_->WritePage(pages_[i].GetPageId(), pages_[i].GetData());
			}
		}
		delete[] pages_;
	}

	bool BufferPoolManager::FindVictimPage(int* frame_id) {
		if (!free_list_.empty()) {
			*frame_id = free_list_.front();
			free_list_.pop_front();
			return true;
		}
		return replacer_.Victim(frame_id);
	}

	Page* BufferPoolManager::FetchPage(int page_id) {
		auto it = page_table_.find(page_id);
		if (it != page_table_.end()) {
			int frame_id = it->second;
			pages_[frame_id].Pin();
			return &pages_[frame_id];
		}
		int frame_id;
		if (!FindVictimPage(&frame_id)) {
			return nullptr; // No available frame
		}
		Page& victim_page = pages_[frame_id];
		if (victim_page.IsDirty()) {
			disk_manager_->WritePage(victim_page.GetPageId(), victim_page.GetData());
		}
		page_table_.erase(victim_page.GetPageId());
		disk_manager_->ReadPage(page_id, victim_page.GetData());
		victim_page.SetPageId(page_id);
		victim_page.Pin();
		victim_page.SetDirty(false);
		page_table_[page_id] = frame_id;
		return &victim_page;
	}

	bool BufferPoolManager::UnpinPage(int page_id, bool is_dirty) {
		
		auto it = page_table_.find(page_id);
		
		if (it == page_table_.end()) {
			return false; // Page not found
		}
		int frame_id = it->second;
		Page& page = pages_[frame_id];

		if(page.GetPinCount() <= 0)
		{
			return false;
		}

		page.UnPin();

		if (is_dirty) {
			page.SetDirty(true);
		}

		if (page.GetPinCount() == 0) {
			replacer_.UnPin(frame_id);
		}

		return true;
	}

	bool BufferPoolManager::FlushPage(int page_id) {
		auto it = page_table_.find(page_id);
		if (it == page_table_.end()) {
			return false; // Page not found
		}

		int frame_id = it->second;
		
		Page& page = pages_[frame_id];
		
		disk_manager_->WritePage(page_id, page.GetData());
		page.SetDirty(false);
		
		return true;
	}

	Page* BufferPoolManager::NewPage(int* page_id) {

		int frame_id;
		if (!FindVictimPage(&frame_id)) {
			return nullptr; // No available frame
		}

		Page& page = pages_[frame_id];

		if (page.IsDirty())
		{
			disk_manager_->WritePage(page.GetPageId(), page.GetData());
		}

		page_table_.erase(page.GetPageId());

		*page_id = disk_manager_->AllocatePage();
		page.ResetMemory();
		page.SetPageId(*page_id);
		page.Pin();
		page.SetDirty(false);
		page_table_[*page_id] = frame_id;

		return &page;

	}
}