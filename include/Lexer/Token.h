#pragma once
#include <string>
#include "../Common/Position.h"
#include "Constant.h"

using std::string;

namespace Basic
{
	class Token
	{
	public:
		// KEYWORD & IDENTIFIER
		Token(const string &type_, const string &value_, const Position &start = Position(), const Position &end = Position());

		// DIGIT & SIGN
		Token(const string &type_ = "", double value_ = 0, const Position &start = Position(), const Position &end = Position());

		// COPY
		Token(const Token &);

		// 仅判断类别和值是否相同
		bool operator==(const Token &other) const;

		bool matches(const string &, const string &);

		void set_pos(const Position &start, const Position &end);
		double get_number();
		string repr();

		string type;
		string value;
		Position pos_start;
		Position pos_end;
	};
}