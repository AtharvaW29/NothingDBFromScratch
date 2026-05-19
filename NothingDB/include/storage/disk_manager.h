#pragma once

#include <fstream>
#include <string>

#include "common/config.h"

namespace NothingDB
{
	class DiskManager
	{
	public:
		explicit DiskManager(const std::string& db_file);
		~DiskManager();
		void WritePage(int page_id, const char* page_data);
		void ReadPage(int page_id, char* page_data);
		int AllocatePage();
		void ShutDown();

	private:
		std::fstream db_io_;
		std::string db_file_name_;
		int next_page_id_;
	};
}