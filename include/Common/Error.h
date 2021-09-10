#pragma once
#include <string>
#include "Position.h"

using std::string;

namespace Basic
{

	class Error
	{
	public:
		Error(const Position &pos_start, const Position &pos_end, const string &error_name, const string &details);
		virtual string as_string();

	protected:
		Position pos_start;
		Position pos_end;
		string error_name;
		string details;
	};

	string string_with_arrows(const string &content, const Position &start, const Position &end);
}