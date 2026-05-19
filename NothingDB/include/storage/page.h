#pragma once

#include <array>
#include <cstring>

#include "common/config.h"

namespace NothingDB
{
	class Page
	{
	public:
		Page();

		inline char* GetData()
		{

			return data_.data();
		}

		inline const char* GetData() const {

			return data_.data();
		}
		inline int GetPageId() const {
			return page_id_;
		}
		inline void SetPageId(int page_id) {
			page_id_ = page_id;
		}
		inline int GetPinCount() const {

			return pin_count_;
		}
		inline void Pin()
		{
			pin_count_++;
		}
		inline void UnPin()
		{
			if (pin_count_ > 0) {
				pin_count_--;
			}
			// else: log error or throw exception
		}
		inline bool IsDirty() const {
			return is_dirty_;
		}
		inline void SetDirty(bool dirty) {
			is_dirty_ = dirty;
		}

		void ResetMemory();

	private:
		std::array<char, PAGE_SIZE> data_;
		int page_id_;
		int pin_count_;
		bool is_dirty_;
	};
}
