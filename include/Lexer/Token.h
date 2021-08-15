#pragma once
#include <string>
#include "../Common/Position.h"
#include "Constant.h"

using std::string;

namespace Basic {

	class Token
	{
	public:
		// KEYWORD & IDENTIFIER
		Token(string type_, string value_, Position start = Position(), Position end = Position());

		// DIGIT & SIGN
		Token(string type_ = "", double value_ = 0, Position start = Position(), Position end = Position());

		// COPY
		Token(const Token&);

		// 仅判断类别和值是否相同
		bool operator==(const Token& other);

		bool matches(string, string);

		void set_pos(Position start, Position end);
		double get_number();
		string __repr__();

		string type;
		string value;
		Position pos_start;
		Position pos_end;
	};

}