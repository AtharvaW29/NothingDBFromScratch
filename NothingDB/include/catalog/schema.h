#pragma once

#include <vector>

#include "catalog/column.h"

namespace NothingDB {
	class Schema {
	public:
		explicit Schema(const std::vector<Column>& columns) : columns_(columns) {}

		const std::vector<Column>& GetColumns() const {
			return columns_;
		}

		size_t GetColumnCount() const {
			return columns_.size();
		}

	private:
		std::vector<Column> columns_;
	};
}