#pragma once
#include "../Common/Error.h"

namespace Basic {
    class InvalidSyntaxError : public Error
    {
    public:
        InvalidSyntaxError(const Position& start, const Position& end, const string& details);
    };
}