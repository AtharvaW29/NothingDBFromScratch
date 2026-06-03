#pragma once

#include <cstdint>

#include "storage/page.h"
#include "storage/tuple.h"
#include "storage/rid.h"


namespace NothingDB {
	// Used as a structure
	struct TupleInfo {
		uint16_t offset;
		uint16_t size;
		bool is_deleted;
	};

	class TablePage : public Page {
	public:
		TablePage();
		void Init();
		bool InsertTuple(const Tuple& tuple, RID& rid);
		bool GetTuple(uint16_t slot_num, Tuple& tuple);
		bool DeleteTuple(uint16_t slot_num);
		bool UpdateTuple(uint16_t slot_num, const Tuple& new_tuple);
		uint16_t GetTupleCount() const;
		int GetNextPageId() const;
		void SetNextPageId(int page_id);

	private:
		uint16_t* GetFreeSpacePointer();
		uint16_t* GetTupleCountPointer();
		TupleInfo* GetTupleInfo(uint16_t slot_num);
	};
}