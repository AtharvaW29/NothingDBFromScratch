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
#include "index/bplus_tree.h"
#include <storage/rid.h>


using namespace NothingDB;

int main()
{

	BPlusTree<int, RID> bptree;
	bptree.Insert(5, RID(1, 1));
	bptree.Insert(10, RID(2, 2));
	bptree.Insert(15, RID(3, 3));
	bptree.Insert(20, RID(4, 4));
	bptree.Insert(25, RID(5, 5));
	bptree.Insert(30, RID(6, 6));
	bptree.Insert(35, RID(7, 7));

	RID rid;

	if (bptree.Search(20, rid)) { 
		std::cout << rid.GetPageId()
			<< "," << rid.GetSlotNum() << std::endl;
	}

	bptree.PrintTree();

    return 0;
}
