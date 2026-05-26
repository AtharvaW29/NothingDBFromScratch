#include "storage/tuple.h"

#include <cstring>
#include <stdexcept>

namespace NothingDB {
    Tuple::Tuple(
        const std::vector<Value>& values) {

        for (const auto& value : values) {
            switch (value.GetType()) {
			    case TypeId::INTEGER: {
                    int int_value = value.AsInt();

                    char* ptr =
                        reinterpret_cast<char*>(&int_value);

                    data_.insert(
                        data_.end(),
                        ptr,
                        ptr + sizeof(int));
                    break;
			    }

                case TypeId::VARCHAR:{
                    std::string str = value.AsString();

                    uint32_t len =
                        static_cast<uint32_t>(str.size());

                    char* len_ptr =
                        reinterpret_cast<char*>(&len);

                    data_.insert(
                        data_.end(),
                        len_ptr,
                        len_ptr + sizeof(uint32_t));

                    data_.insert(
                        data_.end(),
                        str.begin(),
                        str.end());
                    break;
                }
                
                case TypeId::BOOLEAN:{
					bool bool_val = value.AsBool();
                    data_.push_back(static_cast<char>(bool_val));
                    break;
                }
            }
        }
    }

    const std::vector<char>& Tuple::GetData() const {
        return data_;
    }

	std::vector<Value> Tuple::DeSerialize(
		const Schema& schema) const {

		std::vector<Value> values;

		size_t offset = 0;

		for (const auto& column :
			schema.GetColumns()) {

			switch(column.GetType()){

			case TypeId::INTEGER: {
				if (offset + sizeof(int) > data_.size()) {
					throw std::runtime_error("Tuple data is truncated at INTEGER field");
				}

				int value;

				std::memcpy(
					&value,
					data_.data() + offset,
					sizeof(int));

				offset += sizeof(int);

				values.emplace_back(value);
				break;
				}

			case TypeId::VARCHAR: {
				if (offset + sizeof(uint32_t) > data_.size()) {
					throw std::runtime_error("Tuple data is truncated: cannot read VARCHAR length");
				}

				uint32_t len;

				std::memcpy(
					&len,
					data_.data() + offset,
					sizeof(uint32_t));

				offset += sizeof(uint32_t);

				if (offset + len > data_.size()) {
					throw std::runtime_error("Tuple data is truncated: VARCHAR string data incomplete");
				}

				std::string str(
					data_.data() + offset,
					len);

				offset += len;

				values.emplace_back(str);
				break;
				}

			case TypeId::BOOLEAN: {
				if (offset + sizeof(bool) > data_.size()) {
					throw std::runtime_error("Tuple data is truncated at BOOLEAN field");
				}

				bool value = static_cast<bool>(data_.at(offset));
				offset += 1;
				values.emplace_back(value);
				break;
				}
			}
		}

		return values;
	}
}