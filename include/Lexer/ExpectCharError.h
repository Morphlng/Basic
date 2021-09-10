#pragma once
#include "../Common/Error.h"

namespace Basic
{

	class ExpectCharError : public Error
	{
	public:
		ExpectCharError(const Position &start, const Position &end, const string &message);
	};
}