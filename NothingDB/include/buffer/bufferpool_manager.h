#pragma once

#include <list>
#include <unordered_map>
#include <vector>

#include "buffer/lru_replacer.h"
#include "storage/disk_manager.h"
#include "storage/page.h"

namespace NothingDB{
	class BufferPoolManager {
	public:
		BufferPoolManager(size_t pool_size, DiskManager* disk_manager);
		~BufferPoolManager();
		Page* FetchPage(int page_id);
		bool UnpinPage(int page_id, bool is_dirty);
		bool FlushPage(int page_id);
		Page* NewPage(int* page_id);
	private:
		bool FindVictimPage(int* frmae_id);
	private:
		size_t pool_size_;
		Page* pages_;
		std::unordered_map<int, int> page_table_; // page_id -> frame_id
		std::list<int> free_list_; // list of frame_ids
		LRUReplacer replacer_;
		DiskManager* disk_manager_;
	};
}