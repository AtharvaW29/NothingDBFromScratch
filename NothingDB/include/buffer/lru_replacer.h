#pragma once

#include <list>
#include <unordered_map>

namespace NothingDB
{
	class LRUReplacer
	{
	public:
		explicit LRUReplacer(size_t num_pages);
		~LRUReplacer() = default;
		bool Victim(int* page_id);
		void Pin(int frame_id);
		void UnPin(int frame_id);
		size_t Size();

	private:
		size_t capacity_;
		std::list<int> lru_list_;
		std::unordered_map<int, std::list<int>::iterator> lru_map_;
	};
}