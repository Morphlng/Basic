#pragma once
#include "../Common/Error.h"

namespace Basic {

    class IllegalCharError : public Error
    {
    public:
        IllegalCharError(const Position& start, const Position& end, const string& details);
    };
}