#pragma once

#include "common/config.h"

namespace NothingDB{
	class RID {
	public:
		RID() 
		: page_id_(INVALID_PAGE_ID), 
		  slot_num_(INVALID_PAGE_ID) {}

		RID(
			int page_id, int slot_num
		) : page_id_(page_id), 
			slot_num_(slot_num) {}

		int GetPageId() const {
			return page_id_;
		}

		int GetSlotNum() const {
			return slot_num_;
		}

	private:
		int page_id_;
		int slot_num_;
	};
}