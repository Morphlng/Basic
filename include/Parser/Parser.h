#pragma once

#include "../Lexer/Token.h"
#include "Parse_Result.h"
#include "Node.h"
#include <functional>

using std::function;

namespace Basic
{
	class Parser
	{
	public:
		Parser(const vector<Token> &toks);
		void advance();
		void reverse(int amount = 1);
		Parse_Result parse();

		Parse_Result func_def(); // 函数定义

		Parse_Result while_expr(); // while循环
		Parse_Result for_expr();   // for循环

		Parse_Result if_expr_cases(const string &); // if条件语句
		Parse_Result elif_or_else_expr();			// elxx语句
		Parse_Result else_expr();					// else语句
		Parse_Result elif_expr();					// elif语句
		Parse_Result if_expr();						// if语句

		Parse_Result list_expr(); // 列表
		Parse_Result atom();	  // 原子项，包含以上所有节点
		Parse_Result index();	  // 索引语句，可以对列表索引
		Parse_Result ref();		  // 引用变量，表示对变量的引用
		Parse_Result call();	  // 调用语句

		Parse_Result power();	   // 幂运算
		Parse_Result factor();	   // 单元运算
		Parse_Result term();	   // 乘除运算
		Parse_Result arith_expr(); // 加减运算
		Parse_Result comp_expr();  // 布尔运算

		Parse_Result expr();	   // 表达式
		Parse_Result statement();  // 单条语句
		Parse_Result statements(); // 语句合集

		Parse_Result bin_op(function<Parse_Result(Parser *)>, const vector<string> &ops, function<Parse_Result(Parser *)>);
		Parse_Result bin_op(function<Parse_Result(Parser *)>, const vector<Token> &ops, function<Parse_Result(Parser *)>);

	private:
		vector<Token> tokens;
		int tok_idx;
		Token current_tok;
	};
}