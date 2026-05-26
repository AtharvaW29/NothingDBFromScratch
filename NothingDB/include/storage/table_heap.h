#pragma once

#include<vector>

#include "buffer/bufferpool_manager.h"
#include "storage/table_page.h"
#include "storage/tuple.h"


namespace NothingDB{
	class TableHeap {
	public:
		explicit TableHeap(BufferPoolManager* buffer_pool_manager);

		bool InsertTuple(const Tuple& tuple);

		std::vector<Tuple> Scan();

	private:
		BufferPoolManager* buffer_pool_manager_;
		int first_page_id_;
	};
}