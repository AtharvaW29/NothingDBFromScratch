#include "storage/tuple.h"

#include <cstring>

namespace NothingDB {
    Tuple::Tuple(
        const std::vector<Value>& values) {

        for (const auto& value : values) {

            if (value.GetType() == TypeId::INTEGER) {

                int int_value = value.AsInt();

                char* ptr =
                    reinterpret_cast<char*>(&int_value);

                data_.insert(
                    data_.end(),
                    ptr,
                    ptr + sizeof(int));
            }
            else if (value.GetType() == TypeId::VARCHAR) {

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

            if (column.GetType() ==
                TypeId::INTEGER) {

                int value;

                std::memcpy(
                    &value,
                    data_.data() + offset,
                    sizeof(int));

                offset += sizeof(int);

                values.emplace_back(value);
            }
            else if (column.GetType() ==
                TypeId::VARCHAR) {

                uint32_t len;

                std::memcpy(
                    &len,
                    data_.data() + offset,
                    sizeof(uint32_t));

                offset += sizeof(uint32_t);

                std::string str(
                    data_.data() + offset,
                    len);

                offset += len;

                values.emplace_back(str);
            }
        }

        return values;
    }
}