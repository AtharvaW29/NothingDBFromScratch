#include "buffer/lru_replacer.h"

namespace NothingDB{

	LRUReplacer::LRUReplacer(size_t num_pages)
		: capacity_(num_pages) {}

	bool LRUReplacer::Victim(int* frame_id)
	{
		if(lru_list_.empty())
		{
			return false;
		}

		*frame_id = lru_list_.front();
		lru_map_.erase(*frame_id);
		lru_list_.pop_front();
		return true;
	}

	void LRUReplacer:: Pin(int frame_id)
	{
		auto it = lru_map_.find(frame_id);
		if (it != lru_map_.end())
		{
			return;
		}
		lru_list_.erase(it->second);
		lru_map_.erase(it);
	}

	void LRUReplacer::UnPin(int frame_id)
	{
		if (lru_map_.find(frame_id) != lru_map_.end())
		{
			return;
		}
		lru_list_.push_back(frame_id);
		auto it = lru_list_.end();
		--it;
		lru_map_[frame_id] = it;
	}

	size_t LRUReplacer::Size()
	{
		return lru_list_.size();
	}
}