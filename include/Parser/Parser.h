#pragma once

#include "../Lexer/Token.h"
#include "Parse_Result.h"
#include "Node.h"
#include <functional>

using std::function;

namespace Basic {

	class Parser
	{
	public:
		Parser(const vector<Token>& toks);
		void advance();
		void reverse(int amount = 1);
		Parse_Result parse();

		Parse_Result func_def();

		Parse_Result while_expr();
		Parse_Result for_expr();

		Parse_Result if_expr_cases(const string&);
		Parse_Result elif_or_else_expr();
		Parse_Result else_expr();
		Parse_Result elif_expr();
		Parse_Result if_expr();

		Parse_Result list_expr();
		Parse_Result atom();
		Parse_Result call();

		Parse_Result power();
		Parse_Result factor();
		Parse_Result term();
		Parse_Result arith_expr();
		Parse_Result comp_expr();

		Parse_Result expr();	   // 表达式
		Parse_Result statement();  // 单条语句
		Parse_Result statements(); // 语句合集

		Parse_Result bin_op(function<Parse_Result(Parser*)>, const vector<string>& ops, function<Parse_Result(Parser*)>);
		Parse_Result bin_op(function<Parse_Result(Parser*)>, const vector<Token>& ops, function<Parse_Result(Parser*)>);

	private:
		vector<Token> tokens;
		int tok_idx;
		Token current_tok;
	};
}