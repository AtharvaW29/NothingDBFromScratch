// NothingDB.cpp : Defines the entry point for the application.
//

#include "NothingDB.h"
#include "storage/page.h"
#include "storage/disk_manager.h"
#include "buffer/lru_replacer.h"
#include "buffer/bufferpool_manager.h"
#include "storage/table_page.h"
#include "catalog/schema.h"
#include "storage/tuple.h"


using namespace NothingDB;

int main()
{
	Schema schema({
		Column("id", TypeId::INTEGER),
		Column("name", TypeId::VARCHAR),
		Column("is_active", TypeId::BOOLEAN)
		});

	Tuple tuple({
		Value(1),
		Value(std::string("Alice")),
		Value(true)
	});

	try {
		auto values = tuple.DeSerialize(schema);

		std::cout << "id: " << values[0].AsInt() << std::endl;
		std::cout << "name: " << values[1].AsString() << std::endl;
		std::cout << "isactive: " << values[2].AsBool() << std::endl;
	} catch (const std::runtime_error& e) {
		std::cerr << "DeSerialize failed: " << e.what() << std::endl;
		return 1;
	}


    return 0;
}
