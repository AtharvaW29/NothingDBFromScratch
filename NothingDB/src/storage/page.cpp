#include "storage/page.h"

namespace NothingDB
{
	Page::Page()
		: page_id_(INVALID_PAGE_ID), pin_count_(0), is_dirty_(false) {
		ResetMemory();
		}
	void Page::ResetMemory() {
		std::memset(data_.data(), 0, PAGE_SIZE);
	}
}