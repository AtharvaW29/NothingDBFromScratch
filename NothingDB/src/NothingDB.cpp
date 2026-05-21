// NothingDB.cpp : Defines the entry point for the application.
//

#include "NothingDB.h"
#include "storage/page.h"
#include "storage/disk_manager.h"
#include "buffer/lru_replacer.h"
#include "buffer/bufferpool_manager.h"

using namespace NothingDB;

int main()
{

    DiskManager disk("nothing.db");

    BufferPoolManager bpm(2, &disk);

    int page_id1;
    Page* page1 = bpm.NewPage(&page_id1);

    std::strcpy(page1->GetData(), "Hello Page 1");

    bpm.UnpinPage(page_id1, true);

    int page_id2;
    Page* page2 = bpm.NewPage(&page_id2);

    std::strcpy(page2->GetData(), "Hello Page 2");

    bpm.UnpinPage(page_id2, true);

    Page* fetched = bpm.FetchPage(page_id1);

    std::cout << fetched->GetData() << std::endl;

    bpm.UnpinPage(page_id1, false);

    return 0;
}
