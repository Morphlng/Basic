#pragma once
#include <vector>
#include <string>
#include "../Common/Position.h"
#include "Token.h"
#include "IllegalCharError.h"
#include "ExpectCharError.h"

using std::string;
using std::vector;

namespace Basic {

	class Lexer
	{
	public:
		Lexer(string filename, string text);
		void advance();
		void skip_comment();

		vector<Token> make_tokens();

		Token make_sign();
		Token make_number();
		Token make_identifier();

		Token make_string();
		Token make_minus_or_arrow();

		// Token for bool
		Token make_not_equals();
		Token make_equals();
		Token make_less_than();
		Token make_greater_than();

	private:
		string text;
		string filename;
		Position pos;
		char current_char;
	};

}