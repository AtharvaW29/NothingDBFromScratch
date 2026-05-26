// NothingDB.cpp : Defines the entry point
#include<array>;

#include "NothingDB.h"
#include "storage/page.h"
#include "storage/disk_manager.h"
#include "buffer/lru_replacer.h"
#include "buffer/bufferpool_manager.h"
#include "storage/table_page.h"
#include "catalog/schema.h"
#include "storage/tuple.h"
#include "storage/table_heap.h"


using namespace NothingDB;

int main()
{
	DiskManager disk("nothing.db");

	BufferPoolManager bpm(10, &disk);

	TableHeap table(&bpm);

	Schema schema({
		Column("id", TypeId::INTEGER),
		Column("name", TypeId::VARCHAR),
		Column("is_active", TypeId::BOOLEAN)
		});

	table.InsertTuple(Tuple({
		Value(1),
		Value(std::string("Alice")),
		Value(true)
		}));
	
	table.InsertTuple(Tuple({
		Value(2),
		Value(std::string("Bob")),
		Value(false)
		}));

	
	auto tuples = table.Scan();
	std::cout << "id | name | is_active" << std::endl;
	for(auto& tuple: tuples)
	{
		{
			auto values =
				tuple.DeSerialize(schema);

			std::cout
				<< values[0].AsInt()
				<< " | "
				<< values[1].AsString()
				<< " | "
				<< values[2].AsBool()
				<< std::endl;
		}
	}

    return 0;
}
