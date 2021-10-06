#include "Lexer/ExpectCharError.h"

namespace Basic
{

	ExpectCharError::ExpectCharError(const Position &start, const Position &end, const string &details) : Error(start, end, "Expect Character: ", details)
	{
	}

}