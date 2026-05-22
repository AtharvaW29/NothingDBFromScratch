#pragma once

#include <string>

#include "common/types.h"

namespace NothingDB{
    class Column {
    public:
        Column(
            const std::string& name,
            TypeId type)
            : name_(name),
            type_(type) {
        }

        const std::string& GetName() const {
            return name_;
        }

        TypeId GetType() const {
            return type_;
        }

    private:
        std::string name_;

        TypeId type_;
    };
}