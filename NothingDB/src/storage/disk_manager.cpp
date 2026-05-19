#include "storage/disk_manager.h"

#include <cstring>
#include <iostream>

namespace NothingDB
{
	DiskManager::DiskManager(const std::string& db_file)
		: db_file_name_(db_file), next_page_id_(0) {
		db_io_.open(db_file_name_, std::ios::in | std::ios::out | std::ios::binary);
		if (!db_io_.is_open()) {
			db_io_.clear();

			db_io_.open(db_file_name_, std::ios::out | std::ios::binary);
			
			db_io_.close();
			
			db_io_.open(db_file_name_, std::ios::in | std::ios::out | std::ios::binary);
		}

		if (!db_io_.is_open()) {
			std::cerr << "Failed to open or create database file: " << db_file_name_ << std::endl;
			throw std::runtime_error("DiskManager initialization failed");
		}
	}

	DiskManager::~DiskManager() {
		ShutDown();
	}

	void DiskManager::WritePage(int page_id, const char* page_data) {
		std::size_t offset = static_cast<std::size_t>(page_id) * PAGE_SIZE;

		db_io_.seekp(offset);
		db_io_.write(page_data, PAGE_SIZE);
		db_io_.flush();
	}

	void DiskManager::ReadPage(int page_id, char* page_data) {
		std::size_t offset = static_cast<std::size_t>(page_id) * PAGE_SIZE;
		
		db_io_.seekg(offset);
		db_io_.read(page_data, PAGE_SIZE);

		std::size_t bytes_read = db_io_.gcount();

		if (bytes_read < PAGE_SIZE) {
			std::memset(page_data + bytes_read, 0, PAGE_SIZE - bytes_read);
		}
	}

	int DiskManager::AllocatePage() {
		return next_page_id_++;
	}

	void DiskManager::ShutDown() {
		if (db_io_.is_open()) {
			db_io_.close();
		}
	}
}