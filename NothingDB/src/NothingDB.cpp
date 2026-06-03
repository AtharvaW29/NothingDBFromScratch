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
        Column("active", TypeId::BOOLEAN)
        });

    RID rid1;
    RID rid2;

    table.InsertTuple(
        Tuple({
            Value(1),
            Value("Alice"),
            Value(true)
            }),
        rid1);

    table.InsertTuple(
        Tuple({
            Value(2),
            Value("Bob"),
            Value(true)
            }),
        rid2);

    table.UpdateTuple(
        rid1,
        Tuple({
            Value(1),
            Value("Alice --inactive"),
            Value(false)
            }));

    //table.DeleteTuple(rid2);

    auto tuples = table.Scan();

    for (auto& tuple : tuples) {

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

    return 0;
}
