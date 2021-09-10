#include "../../include/Lexer/IllegalCharError.h"

namespace Basic
{

	IllegalCharError::IllegalCharError(const Position &start, const Position &end, const string &details) : Error(start, end, "Illegal Character", details)
	{
	}

}