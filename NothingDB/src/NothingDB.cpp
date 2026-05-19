// NothingDB.cpp : Defines the entry point for the application.
//

#include "NothingDB.h"
#include "storage/page.h"
#include "storage/disk_manager.h"

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

	std::cout << "Read from disk: " 
			  << read_page.GetData() 
			  << std::endl;

    return 0;
}
