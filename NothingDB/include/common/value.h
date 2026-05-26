#pragma once

#include <string>
#include <variant>

#include "common/types.h"

namespace NothingDB {
    class Value {
    public:
        Value(int value) : type_(TypeId::INTEGER),
			value_(value) {}

		Value(const std::string& value) : type_(TypeId::VARCHAR),
            value_(value) {}

		Value(bool value) : type_(TypeId::BOOLEAN),
			value_(value) {}
		
		TypeId GetType() const {
			return type_;
		}

		int AsInt() const {
			return std::get<int>(value_);
		}

		std::string AsString() const {
			return std::get<std::string>(value_);
		}

		bool AsBool() const {
			return std::get<bool>(value_);
		}

	private:
		TypeId type_;
		std::variant<int, std::string, bool> value_;
    };
}