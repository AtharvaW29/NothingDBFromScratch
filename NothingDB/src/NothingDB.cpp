// NothingDB.cpp : Defines the entry point for the application.
//

#include "NothingDB.h"
#include "storage/page.h"
#include "storage/disk_manager.h"
#include "buffer/lru_replacer.h"

using namespace NothingDB;

int main()
{
	DiskManager disk("nothing.db");

	int page_id = disk.AllocatePage();

	Page write_page;

	write_page.SetPageId(page_id);

	std::strcpy(write_page.GetData(), "Hello, NothingDB! This is a Persistant Database Page.");
	disk.WritePage(write_page.GetPageId(), write_page.GetData());

	Page read_page;

	disk.ReadPage(page_id, read_page.GetData());

	LRUReplacer lru(3);

	lru.UnPin(1);
	lru.UnPin(2);
	lru.UnPin(3);

	int victim = 2;

	lru.Victim(&victim);

	std::cout << "Read from disk: "
		<< read_page.GetData()
		<< std::endl;

	for (int i = 0; i < 3; ++i) {
		std::cout << "LRU Replacer Frame " << i << ": "
			<< (lru.Size() > 0 ? "Unpinned" : "Pinned")
			<< std::endl;
	}

	std::cout << "Vicitim Frame:"
		<< victim
		<< std::endl;

	lru.Pin(2);

	for (int i = 0; i < 3; ++i) {
		std::cout << "LRU Replacer Frame " << i << ": "
			<< (lru.Size() > 0 ? "Unpinned" : "Pinned")
			<< std::endl;
	}

	std::cout << "Replacer Size:"
		<< lru.Size()

		<< std::endl;

	return 0;
}
