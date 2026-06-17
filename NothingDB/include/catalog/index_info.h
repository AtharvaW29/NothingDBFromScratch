#pragma once

#include <string>

namespace NothingDB
{
	class IndexInfo {
	public:
		IndexInfo(
			const std::string& name,
			const std::string& column_name)
			: name_(name), column_name_(column_name)
		{
		}
		std::string name_;
		std::string column_name_;
	};
}