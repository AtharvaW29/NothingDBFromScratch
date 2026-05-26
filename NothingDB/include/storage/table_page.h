#pragma once

#include <cstdint>

#include "storage/page.h"
#include "storage/tuple.h"

namespace NothingDB {
	// Used it earlier as a temp structure
	struct TupleInfo {
		uint16_t offset;
		uint16_t size;
	};

	class TablePage : public Page {
	public:
		TablePage();
		void Init();
		bool InsertTuple(const Tuple& tuple);
		bool GetTuple(uint16_t slot_num, Tuple& tuple);
		uint16_t GetTupleCount() const;
		int GetNextPageId() const;
		void SetNextPageId(int page_id);

	private:
		uint16_t* GetFreeSpacePointer();
		uint16_t* GetTupleCountPointer();
		TupleInfo* GetTupleInfo(uint16_t slot_num);
	};
}