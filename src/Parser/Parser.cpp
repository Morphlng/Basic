#include "../../include/Common/utils.h"
#include "../../include/Parser/Parser.h"
#include "../../include/Parser/InvalidSyntaxError.h"

using std::make_shared;
using std::make_tuple;

namespace Basic {

	Parser::Parser(vector<Token> toks)
	{
		this->tokens = toks;
		this->tok_idx = -1;
		advance();
	}

	void Parser::advance()
	{
		this->tok_idx++;
		if (tok_idx < tokens.size())
			current_tok = tokens[tok_idx];
	}

	void Parser::reverse(int amount)
	{
		this->tok_idx -= amount;
		if (tok_idx >= 0 && tok_idx < tokens.size())
			current_tok = tokens[tok_idx];
	}

	Parse_Result Parser::parse()
	{
		Parse_Result res = statements();

		// 若解析未出错，但依旧没有到达EOF，说明存在SyntaxError
		if (!res.hasError() && this->current_tok.type != TD_EOF)
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected '+', '-', '*', '/', '^', '==', '!=', '<', '>', <=', '>=', 'AND' or 'OR'"));
		}

		return res;
	}

	Parse_Result Parser::func_def()
	{
		Parse_Result res;

		if (!current_tok.matches(TD_KEYWORD, "FUNC"))
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected 'FUNC'"));
		}

		res.registry_advancement();
		advance();

		bool anonymous = false;
		Token var_name(TD_IDENTIFIER, "<anonymous>");
		if (current_tok.type == TD_IDENTIFIER)
		{
			var_name = current_tok;
			res.registry_advancement();
			advance();

			if (current_tok.type != TD_LPAREN)
			{
				return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected '('"));
			}
		}
		else
		{
			anonymous = true;
			if (current_tok.type != TD_LPAREN)
			{
				return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected identifier or '('"));
			}
		}

		res.registry_advancement();
		advance();

		vector<Token> arg_name_toks;

		// 有参数
		if (current_tok.type == TD_IDENTIFIER)
		{
			arg_name_toks.push_back(current_tok);
			res.registry_advancement();
			advance();

			while (current_tok.type == TD_COMMA)
			{
				res.registry_advancement();
				advance();

				if (current_tok.type != TD_IDENTIFIER)
				{
					return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected identifier"));
				}

				arg_name_toks.push_back(current_tok);
				res.registry_advancement();
				advance();
			}

			if (current_tok.type != TD_RPAREN)
			{
				return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected ',' or ')'"));
			}
		}
		// 无参数
		else
		{
			if (current_tok.type != TD_RPAREN)
			{
				return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected identifier or ')'"));
			}
		}

		res.registry_advancement();
		advance();

		// 单行函数定义
		if (current_tok.type == TD_ARROW)
		{
			res.registry_advancement();
			advance();

			shared_ptr<ASTNode> exp = res.registry(statement());
			if (res.hasError())
				return res;

			return res.success(make_shared<FuncDefNode>(var_name, arg_name_toks, exp, anonymous, true));
		}

		// 多行函数定义
		if (current_tok.type != TD_COLON && current_tok.type != TD_NEWLINE)
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected '->' or NEWLINE"));
		}
		res.registry_advancement();
		advance();

		shared_ptr<ASTNode> body = res.registry(statements());
		if (res.hasError())
			return res;

		if (!current_tok.matches(TD_KEYWORD, "END"))
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected 'END'"));
		}

		res.registry_advancement();
		advance();

		return res.success(make_shared<FuncDefNode>(var_name, arg_name_toks, body, anonymous, false));
	}

	Parse_Result Parser::while_expr()
	{
		Parse_Result res;

		if (!current_tok.matches(TD_KEYWORD, "WHILE"))
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected 'WHILE'"));
		}

		res.registry_advancement();
		advance();

		shared_ptr<ASTNode> condition = res.registry(expr());
		if (res.hasError())
			return res;

		if (!current_tok.matches(TD_KEYWORD, "THEN"))
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected 'THEN'"));
		}

		res.registry_advancement();
		advance();

		// 下面开始获取循环主体(body)
		shared_ptr<ASTNode> body = nullptr;
		// 多行情况
		if (current_tok.type == TD_COLON || current_tok.type == TD_NEWLINE)
		{
			res.registry_advancement();
			advance();

			body = res.registry(statements());
			if (res.hasError())
				return res;

			if (!current_tok.matches(TD_KEYWORD, "END"))
			{
				return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected 'END'"));
			}

			res.registry_advancement();
			advance();

			return res.success(make_shared<WhileNode>(condition, body, true));
		}

		// 单行情况
		body = res.registry(statement());
		if (res.hasError())
			return res;

		return res.success(make_shared<WhileNode>(condition, body, false));
	}

	Parse_Result Parser::for_expr()
	{
		Parse_Result res;

		// 先确定循环符合语法
		if (!current_tok.matches(TD_KEYWORD, "FOR"))
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected 'FOR'"));
		}

		res.registry_advancement();
		advance();

		if (current_tok.type != TD_IDENTIFIER)
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected identifier"));
		}

		Token var_name = this->current_tok;

		res.registry_advancement();
		advance();

		if (current_tok.type != TD_EQ)
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected '='"));
		}

		res.registry_advancement();
		advance();

		shared_ptr<ASTNode> start_value = res.registry(expr());
		if (res.hasError())
			return res;

		if (!current_tok.matches(TD_KEYWORD, "TO"))
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected 'TO'"));
		}

		res.registry_advancement();
		advance();

		shared_ptr<ASTNode> end_value = res.registry(expr());
		if (res.hasError())
			return res;

		// 步长设置可选
		shared_ptr<ASTNode> step_value = nullptr;
		if (current_tok.matches(TD_KEYWORD, "STEP"))
		{
			res.registry_advancement();
			advance();

			step_value = res.registry(expr());
			if (res.hasError())
				return res;
		}

		if (!current_tok.matches(TD_KEYWORD, "THEN"))
		{
			return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected 'THEN'"));
		}

		res.registry_advancement();
		advance();

		// 下面开始获取循环主体(body)
		shared_ptr<ASTNode> body = nullptr;
		// 多行情况
		if (current_tok.type == TD_COLON || current_tok.type == TD_NEWLINE)
		{
			res.registry_advancement();
			advance();

			body = res.registry(statements());
			if (res.hasError())
				return res;

			if (!current_tok.matches(TD_KEYWORD, "END"))
			{
				return res.failure(make_shared<InvalidSyntaxError>(this->current_tok.pos_start, this->current_tok.pos_end, "Expected 'END'"));
			}

			res.registry_advancement();
			advance();

			return res.success(make_shared<ForNode>(var_name, start_value, end_value, body, step_value, true));
		}

		// 单行情况
		body = res.registry(statement());
		if (res.hasError())
			return res;
		return res.success(make_shared<ForNode>(var_name, start_value, end_value, body, step_value, false));
	}

	Parse_Result Parser::if_expr_cases(string case_keyword)
	{
		Parse_Result res;
		Cases cases;
		Else_Case else_case;

		if (!current_tok.matches(TD_KEYWORD, case_keyword))
		{
			return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected " + case_keyword));
		}

		res.registry_advancement();
		advance();

		shared_ptr<ASTNode> condition = res.registry(expr());
		if (res.hasError())
			return res;

		if (!current_tok.matches(TD_KEYWORD, "THEN"))
		{
			return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected 'THEN'"));
		}

		res.registry_advancement();
		advance();

		if (current_tok.type == TD_COLON)
		{
			res.registry_advancement();
			advance();

			shared_ptr<ASTNode> statement_node = res.registry(statements());
			if (res.hasError())
				return res;

			// 不允许多行表达式作为另一个表达式的一部分
			// 例如，对于单行表达式可以有
			// VAR a = IF age<18 THEN "Child" ELSE "Adult"
			// 但是多行不允许，所以传入一个bool值来使其返回null
			cases.push_back(std::make_tuple(condition, statement_node, true));

			if (current_tok.matches(TD_KEYWORD, "END"))
			{
				res.registry_advancement();
				advance();
			}
			else
			{
				shared_ptr<ASTNode> all_cases = res.registry(elif_or_else_expr());
				if (res.hasError())
					return res;

				shared_ptr<IfNode> all_cases_node = std::dynamic_pointer_cast<IfNode>(all_cases);
				Cases new_cases = all_cases_node->get_cases();
				cases.insert(cases.end(), new_cases.begin(), new_cases.end());
				else_case = all_cases_node->get_else_case();
			}
		}
		else
		{
			shared_ptr<ASTNode> exp = res.registry(statement());
			if (res.hasError())
				return res;

			while (current_tok.type == TD_NEWLINE)
			{
				res.registry_advancement();
				advance();
			}

			// 单行表达式，可以利用其返回值
			cases.push_back(std::make_tuple(condition, exp, false));

			shared_ptr<ASTNode> all_cases = res.registry(elif_or_else_expr());
			if (res.hasError())
				return res;

			shared_ptr<IfNode> all_cases_node = std::dynamic_pointer_cast<IfNode>(all_cases);
			Cases new_cases = all_cases_node->get_cases();
			cases.insert(cases.end(), new_cases.begin(), new_cases.end());
			else_case = all_cases_node->get_else_case();
		}

		return res.success(make_shared<IfNode>(cases, else_case));
	}

	Parse_Result Parser::elif_or_else_expr()
	{
		Parse_Result res;
		shared_ptr<IfNode> all_cases_node = nullptr;

		if (current_tok.matches(TD_KEYWORD, "ELIF"))
		{
			shared_ptr<ASTNode> all_cases = res.registry(elif_expr());
			if (res.hasError())
				return res;

			all_cases_node = std::dynamic_pointer_cast<IfNode>(all_cases);
		}
		else
		{
			shared_ptr<ASTNode> else_case = res.registry(else_expr());
			if (res.hasError())
				return res;

			all_cases_node = std::dynamic_pointer_cast<IfNode>(else_case);
		}

		return res.success(all_cases_node);
	}

	Parse_Result Parser::else_expr()
	{
		Parse_Result res;
		Cases empty;
		Else_Case else_case;

		if (current_tok.matches(TD_KEYWORD, "ELSE"))
		{
			res.registry_advancement();
			advance();

			if (current_tok.type == TD_COLON || current_tok.type == TD_NEWLINE)
			{
				res.registry_advancement();
				advance();

				shared_ptr<ASTNode> statement_node = res.registry(statements());
				if (res.hasError())
					return res;

				else_case = make_tuple(statement_node, true);

				if (current_tok.matches(TD_KEYWORD, "END"))
				{
					res.registry_advancement();
					advance();
				}
				else
				{
					return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected 'END'"));
				}
			}
			else
			{
				shared_ptr<ASTNode> exp = res.registry(statement());
				if (res.hasError())
					return res;

				while (current_tok.type == TD_NEWLINE)
				{
					res.registry_advancement();
					advance();
				}

				else_case = make_tuple(exp, false);
			}
		}
		return res.success(make_shared<IfNode>(empty, else_case));
	}

	Parse_Result Parser::elif_expr()
	{
		return if_expr_cases("ELIF");
	}

	Parse_Result Parser::if_expr()
	{
		Parse_Result res;
		shared_ptr<ASTNode> all_cases = res.registry(if_expr_cases("IF"));
		if (res.hasError())
			return res;

		return res.success(all_cases);
	}

	Parse_Result Parser::list_expr()
	{
		Parse_Result res;
		vector<shared_ptr<ASTNode>> elem_nodes;
		Position start = this->current_tok.pos_start;

		if (current_tok.type != TD_LSQUARE)
		{
			return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected '['"));
		}

		res.registry_advancement();
		advance();

		// empty list
		if (current_tok.type == TD_RSQUARE)
		{
			res.registry_advancement();
			advance();
		}
		else
		{
			elem_nodes.push_back(res.registry(expr()));
			if (res.hasError())
			{
				return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected ']', 'VAR', 'IF', 'FOR', 'WHILE', 'FUNC', int, float, identifier, '+', '-', '(', '[' or 'NOT'"));
			}

			while (current_tok.type == TD_COMMA)
			{
				res.registry_advancement();
				advance();

				elem_nodes.push_back(res.registry(expr()));
				if (res.hasError())
					return res;
			}

			if (current_tok.type != TD_RSQUARE)
			{
				return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected ',' or ']'"));
			}

			res.registry_advancement();
			advance();
		}

		return res.success(make_shared<ListNode>(elem_nodes, start, current_tok.pos_end));
	}

	Parse_Result Parser::atom()
	{
		Parse_Result res;
		Token tok = this->current_tok;

		vector<string> TYPES{ TD_INT, TD_FLOAT };

		if (Basic::isIn(TYPES, tok.type))
		{
			res.registry_advancement();
			advance();

			return res.success(make_shared<NumberNode>(tok));
		}
		else if (tok.type == TD_STRING)
		{
			res.registry_advancement();
			advance();

			return res.success(make_shared<StringNode>(tok));
		}
		else if (tok.type == TD_IDENTIFIER)
		{
			res.registry_advancement();
			advance();
			return res.success(make_shared<VarAccessNode>(tok));
		}
		else if (tok.type == TD_LPAREN)
		{
			res.registry_advancement();
			advance();
			shared_ptr<ASTNode> exp = res.registry(expr());
			if (res.hasError())
				return res;

			if (current_tok.type == TD_RPAREN)
			{
				res.registry_advancement();
				advance();
				return res.success(exp);
			}
			else
				return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected ')'"));
		}
		else if (tok.type == TD_LSQUARE)
		{
			shared_ptr<ASTNode> list_exp = res.registry(list_expr());
			if (res.hasError())
				return res;
			return res.success(list_exp);
		}
		else if (tok.matches(TD_KEYWORD, "IF"))
		{
			shared_ptr<ASTNode> if_exp = res.registry(if_expr());
			if (res.hasError())
				return res;
			return res.success(if_exp);
		}
		else if (tok.matches(TD_KEYWORD, "FOR"))
		{
			shared_ptr<ASTNode> for_exp = res.registry(for_expr());
			if (res.hasError())
				return res;
			return res.success(for_exp);
		}
		else if (tok.matches(TD_KEYWORD, "WHILE"))
		{
			shared_ptr<ASTNode> while_exp = res.registry(while_expr());
			if (res.hasError())
				return res;
			return res.success(while_exp);
		}
		else if (tok.matches(TD_KEYWORD, "FUNC"))
		{
			shared_ptr<ASTNode> func_exp = res.registry(func_def());
			if (res.hasError())
				return res;
			return res.success(func_exp);
		}

		// 这个错误信息，同时处理了atom和factor
		return res.failure(make_shared<InvalidSyntaxError>(tok.pos_start, tok.pos_end, "Expected int, float, identifier, '+', '-', '(', '[', 'IF', 'FOR', 'WHILE', 'FUNC'"));
	}

	Parse_Result Parser::call()
	{
		Parse_Result res;
		shared_ptr<ASTNode> atom_node = res.registry(atom());
		if (res.hasError())
			return res;

		if (current_tok.type == TD_LPAREN)
		{
			res.registry_advancement();
			advance();

			vector<shared_ptr<ASTNode>> arg_nodes;

			if (current_tok.type == TD_RPAREN)
			{
				res.registry_advancement();
				advance();
			}
			else
			{
				arg_nodes.push_back(res.registry(expr()));
				if (res.hasError())
				{
					return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected ')', 'VAR', 'IF', 'FOR', 'WHILE', 'FUNC', int, float, identifier, '+', '-', '(', '[' or 'NOT'"));
				}

				while (current_tok.type == TD_COMMA)
				{
					res.registry_advancement();
					advance();

					arg_nodes.push_back(res.registry(expr()));
					if (res.hasError())
						return res;
				}

				if (current_tok.type != TD_RPAREN)
				{
					return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected ',' or ')'"));
				}

				res.registry_advancement();
				advance();
			}

			return res.success(make_shared<CallNode>(atom_node, arg_nodes));
		}
		else if (current_tok.type == TD_LSQUARE)
		{
			shared_ptr<ASTNode> result = atom_node;
			shared_ptr<ASTNode> index_node;

			while (current_tok.type == TD_LSQUARE)
			{
				res.registry_advancement();
				advance();

				index_node = res.registry(expr());
				if (res.hasError())
					return res;

				if(current_tok.type!=TD_RSQUARE)
				{
					return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected ']'"));
				}

				res.registry_advancement();
				advance();

				result = make_shared<IndexNode>(result, index_node);
			}

			return res.success(result);
		}

		return res.success(atom_node);
	}

	Parse_Result Parser::power()
	{
		vector<string> OPS{ TD_POW };
		return bin_op(&Parser::call, OPS, &Parser::factor);
	}

	Parse_Result Parser::factor()
	{
		Parse_Result res;
		Token tok = this->current_tok;

		vector<string> OPS{ TD_PLUS, TD_MINUS };

		// 一元运算，如：+5、-5
		if (Basic::isIn(OPS, tok.type))
		{
			res.registry_advancement();
			advance();
			shared_ptr<ASTNode> fac = res.registry(factor());
			if (res.hasError())
				return res;
			return res.success(make_shared<UnaryOpNode>(tok, fac));
		}

		return power();
	}

	Parse_Result Parser::term()
	{
		vector<string> OPS{ TD_MUL, TD_DIV };
		return bin_op(&Parser::factor, OPS, &Parser::factor);
	}

	Parse_Result Parser::arith_expr()
	{
		vector<string> OPS{ TD_PLUS, TD_MINUS };
		return bin_op(&Parser::term, OPS, &Parser::term);
	}

	Parse_Result Parser::comp_expr()
	{
		Parse_Result res;

		if (this->current_tok.matches(TD_KEYWORD, "NOT"))
		{
			Token op_tok = this->current_tok;

			res.registry_advancement();
			advance();

			shared_ptr<ASTNode> node = res.registry(comp_expr());
			if (res.hasError())
				return res;

			return res.success(make_shared<UnaryOpNode>(op_tok, node));
		}

		vector<string> OPS{ TD_EE, TD_NE, TD_LT, TD_GT, TD_LTE, TD_GTE };
		shared_ptr<ASTNode> node = res.registry(bin_op(&Parser::arith_expr, OPS, &Parser::arith_expr));

		if (res.hasError())
			return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected int, float, identifier, '+', '-', '(', '[' or 'NOT'"));

		return res.success(node);
	}

	Parse_Result Parser::expr()
	{
		Parse_Result res;
		if (this->current_tok.matches(TD_KEYWORD, "VAR"))
		{
			res.registry_advancement();
			advance();
			if (this->current_tok.type != TD_IDENTIFIER)
				return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected identifier"));

			Token var_name = current_tok;
			res.registry_advancement();
			advance();

			shared_ptr<ASTNode> mutant = make_shared<VarAccessNode>(var_name);
			
			// 若在修改列表中的值，则创建MutateNode
			// 否则为AssignNode
			while (current_tok.type == TD_LSQUARE)
			{
				res.registry_advancement();
				advance();

				shared_ptr<ASTNode> index_node = res.registry(expr());
				if (res.hasError())
					return res;

				if (current_tok.type != TD_RSQUARE)
				{
					return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected ']'"));
				}

				res.registry_advancement();
				advance();

				mutant = make_shared<IndexNode>(mutant, index_node);
			}

			if (this->current_tok.type != TD_EQ)
				return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected '='"));
			
			res.registry_advancement();
			advance();

			shared_ptr<ASTNode> exp = res.registry(expr());
			if (res.hasError())
				return res;

			if (typeid(*mutant) == typeid(IndexNode))
				return res.success(make_shared<MutateNode>(mutant, exp));

			return res.success(make_shared<VarAssignNode>(var_name, exp));
		}

		vector<Token> LOGIC{ Token(TD_KEYWORD, "AND"), Token(TD_KEYWORD, "OR") };
		shared_ptr<ASTNode> node = res.registry(bin_op(&Parser::comp_expr, LOGIC, &Parser::comp_expr));

		if (res.hasError())
			return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected 'VAR', 'IF', 'FOR', 'WHILE', 'FUNC', int, float, identifier, '+', '-', '(', '[' or 'NOT'"));

		return res.success(node);
	}

	Parse_Result Parser::statement()
	{
		Parse_Result res;
		Position start = current_tok.pos_start;

		if (current_tok.matches(TD_KEYWORD, "RETURN"))
		{
			res.registry_advancement();
			advance();

			shared_ptr<ASTNode> exp = res.registry(expr());
			if (res.hasError())
				return res;

			if (exp == nullptr)
				reverse(res.reverse_count);

			return res.success(make_shared<ReturnNode>(exp, start, current_tok.pos_start));
		}

		if (current_tok.matches(TD_KEYWORD, "BREAK"))
		{
			res.registry_advancement();
			advance();

			return res.success(make_shared<BreakNode>(start, current_tok.pos_end));
		}

		if (current_tok.matches(TD_KEYWORD, "CONTINUE"))
		{
			res.registry_advancement();
			advance();

			return res.success(make_shared<ContinueNode>(start, current_tok.pos_end));
		}

		shared_ptr<ASTNode> exp = res.registry(expr());
		if (res.hasError())
			return res.failure(make_shared<InvalidSyntaxError>(current_tok.pos_start, current_tok.pos_end, "Expected 'RETURN', 'BREAK', 'CONTINUE', 'VAR', 'IF', 'FOR', 'WHILE', 'FUNC', int, float, identifier, '+', '-', '(', '[' or 'NOT'"));

		return res.success(exp);
	}

	Parse_Result Parser::statements()
	{
		Parse_Result res;
		Position start = this->current_tok.pos_start;
		vector<shared_ptr<ASTNode>> statements; // a list of expression

		while (current_tok.type == TD_NEWLINE)
		{
			res.registry_advancement();
			advance();
		}

		shared_ptr<ASTNode> statement_node = res.registry(statement());
		if (res.hasError())
			return res;

		statements.push_back(statement_node);

		bool more_statements = true;

		while (true)
		{
			int new_linecount = 0;
			while (current_tok.type == TD_NEWLINE)
			{
				res.registry_advancement();
				advance();
				new_linecount++;
			}
			if (new_linecount == 0)
				more_statements = false;

			if (!more_statements)
				break;

			// 尝试去获取一个expression
			// 在这个过程中会使current_tok向前步进
			// 如果失败，我们需要回退
			statement_node = res.try_registry(statement());
			if (statement_node == nullptr)
			{
				reverse(res.reverse_count);
				more_statements = false;
				continue;
			}

			statements.push_back(statement_node);
		}

		if (statements.size() == 1)
			return res.success(statements[0]);
		else
			return res.success(make_shared<ListNode>(statements, start, current_tok.pos_end));
	}

	Parse_Result Parser::bin_op(function<Parse_Result(Parser*)> func_a, const vector<string>& OPS, function<Parse_Result(Parser*)> func_b)
	{
		Parse_Result res;
		shared_ptr<ASTNode> left = res.registry(func_a(this));
		if (res.hasError())
			return res;

		while (Basic::isIn(OPS, current_tok.type))
		{
			Token op_tok = current_tok;
			res.registry_advancement();
			advance();

			shared_ptr<ASTNode> right = res.registry(func_b(this));
			if (res.hasError())
				return res;

			left = make_shared<BinOpNode>(left, op_tok, right);
		}

		return res.success(left);
	}

	Parse_Result Parser::bin_op(function<Parse_Result(Parser*)> func_a, const vector<Token>& OPS, function<Parse_Result(Parser*)> func_b)
	{
		Parse_Result res;
		shared_ptr<ASTNode> left = res.registry(func_a(this));
		if (res.hasError())
			return res;

		while (Basic::isIn(OPS, current_tok))
		{
			Token op_tok = current_tok;
			res.registry_advancement();
			advance();

			shared_ptr<ASTNode> right = res.registry(func_b(this));
			if (res.hasError())
				return res;

			left = make_shared<BinOpNode>(left, op_tok, right);
		}

		return res.success(left);
	}

}