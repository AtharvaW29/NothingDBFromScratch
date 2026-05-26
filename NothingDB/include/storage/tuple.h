#pragma once

#include <vector>

#include "catalog/schema.h"
#include "common/value.h"

namespace NothingDB {
	class Tuple {
	public:
		Tuple() = default;
		explicit Tuple(const std::vector<Value>& values);

		const std::vector<char>& GetData() const;
		void SetData(const std::vector<char>& data);
		std::vector<Value> DeSerialize(const Schema& schema) const;
	private:
		std::vector<char> data_;
	};
}