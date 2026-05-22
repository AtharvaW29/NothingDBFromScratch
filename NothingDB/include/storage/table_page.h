#pragma once

#include <cstdint>

#include "storage/page.h"

namespace NothingDB {
	struct TupleInfo {
		uint16_t offset;
		uint16_t size;
	};

	class TablePage : public Page {
	public:
		TablePage();
		void Init();
		bool InsertTuple(const char* data, uint16_t size);
		bool GetTuple(uint16_t slot_num, char* buffer, uint16_t& size);
		uint16_t GetTupleCount() const;

	private:
		uint16_t* GetFreeSpacePointer();
		uint16_t* GetTupleCountPointer();
		TupleInfo* GetTupleInfo(uint16_t slot_num);
	};
}