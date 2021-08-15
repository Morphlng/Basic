#include "../../include/Parser/InvalidSyntaxError.h"

namespace Basic {

	InvalidSyntaxError::InvalidSyntaxError(const Position& start, const Position& end, const string& details) : Error(start, end, "Invalid Syntax", details)
	{
	}

}