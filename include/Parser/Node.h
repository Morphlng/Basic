#pragma once
#include "../Lexer/Token.h"
#include <memory>
#include <tuple>

using std::shared_ptr;
using std::tuple;

namespace Basic {

	// 结点基类
	class ASTNode
	{
	public:
		virtual string __repr__() = 0;

	public:
		Position pos_start;
		Position pos_end;
	};

	// 数字结点
	class NumberNode : public ASTNode
	{
	public:
		NumberNode(Token tok);
		string __repr__();

		Token get_tok();

	private:
		Token tok;
	};

	// 字符串结点
	class StringNode : public ASTNode
	{
	public:
		StringNode(Token tok);
		string __repr__();

		Token get_tok();

	private:
		Token tok;
	};

	// 列表结点
	class ListNode : public ASTNode
	{
	public:
		ListNode(vector<shared_ptr<ASTNode>>& elem_nodes, Position start = Position(), Position end = Position());

		vector<shared_ptr<ASTNode>> get_element_nodes();
		string __repr__();

	private:
		vector<shared_ptr<ASTNode>> element_nodes;
	};

	// 索引节点，e.g. list[1]
	class IndexNode :public ASTNode
	{
	public:
		IndexNode(shared_ptr<ASTNode> value, shared_ptr<ASTNode> index);

		shared_ptr<ASTNode> get_value();
		shared_ptr<ASTNode> get_index();
		string __repr__();

	private:
		shared_ptr<ASTNode> value;
		shared_ptr<ASTNode> index;
	};

	// 二元运算结点
	class BinOpNode : public ASTNode
	{
	public:
		BinOpNode(shared_ptr<ASTNode> _left, Token _op, shared_ptr<ASTNode> _right);
		string __repr__();

		shared_ptr<ASTNode> get_left();
		shared_ptr<ASTNode> get_right();
		Token get_op();

	private:
		Token op;
		shared_ptr<ASTNode> left;
		shared_ptr<ASTNode> right;
	};

	// 一元运算结点
	class UnaryOpNode : public ASTNode
	{
	public:
		UnaryOpNode(Token _op, shared_ptr<ASTNode> _node);
		string __repr__();

		Token get_op();
		shared_ptr<ASTNode> get_node();

	private:
		Token op;
		shared_ptr<ASTNode> node;
	};

	// 访问变量结点
	class VarAccessNode : public ASTNode
	{
	public:
		VarAccessNode(Token var_name_tok);
		string __repr__();

		Token get_var_name_tok();

	private:
		Token var_name_tok;
	};

	// 可变值节点（不同于Assign，这将直接修改地址指向的值）
	class MutateNode :public ASTNode
	{
	public:
		MutateNode(shared_ptr<ASTNode> mutant, shared_ptr<ASTNode> value);

		string __repr__();

		shared_ptr<ASTNode> get_mutant();
		shared_ptr<ASTNode> get_value();

	private:
		shared_ptr<ASTNode> mutant;	// 要改变的变量
		shared_ptr<ASTNode> value;	// 给定的新值
	};

	// 变量赋值结点
	class VarAssignNode : public ASTNode
	{
	public:
		VarAssignNode(Token var_name_tok, shared_ptr<ASTNode> value_node);
		string __repr__();

		Token get_var_name_tok();
		shared_ptr<ASTNode> get_value_node();

	private:
		Token var_name_tok;
		shared_ptr<ASTNode> value_node;
	};

	using Cases = vector<tuple<shared_ptr<ASTNode>, shared_ptr<ASTNode>, bool>>;
	using Else_Case = tuple<shared_ptr<ASTNode>, bool>;

	// If结点
	class IfNode : public ASTNode
	{
	public:
		IfNode(Cases& _cases, Else_Case& _else_case);
		string __repr__();

		Cases& get_cases();
		Else_Case& get_else_case();

	private:
		// Case: condition, expr, is_return_null
		Cases cases;
		// Else_Case: expr, is_return_null
		Else_Case else_case;
	};

	// For结点
	class ForNode : public ASTNode
	{
	public:
		ForNode(Token var_name, shared_ptr<ASTNode> start_value_node, shared_ptr<ASTNode> end_value_node, shared_ptr<ASTNode> body_node, shared_ptr<ASTNode> step_value_node = nullptr, bool return_null = false);
		string __repr__();

		Token get_var_name_tok();
		shared_ptr<ASTNode> get_start_value_node();
		shared_ptr<ASTNode> get_end_value_node();
		shared_ptr<ASTNode> get_body_node();
		shared_ptr<ASTNode> get_step_value_node();
		bool is_return_null();

	private:
		Token var_name_tok;
		shared_ptr<ASTNode> start_value_node; // 循环起始
		shared_ptr<ASTNode> end_value_node;	  // 循环终点
		shared_ptr<ASTNode> body_node;		  // 循环主体
		shared_ptr<ASTNode> step_value_node;  // 循环步长，默认为1
		bool return_null;					  // 多行循环结构不可做为赋值语句的expr
	};

	// While结点
	class WhileNode : public ASTNode
	{
	public:
		WhileNode(shared_ptr<ASTNode> condition, shared_ptr<ASTNode> body_node, bool return_null = false);
		string __repr__();

		shared_ptr<ASTNode> get_condition_node();
		shared_ptr<ASTNode> get_body_node();
		bool is_return_null();

	private:
		shared_ptr<ASTNode> condition_node;
		shared_ptr<ASTNode> body_node;
		bool return_null;
	};

	// 函数定义结点
	class FuncDefNode : public ASTNode
	{
	public:
		FuncDefNode(Token var_name, vector<Token> arg_name_toks, shared_ptr<ASTNode> body_node, bool anonymous = false, bool auto_return = true);
		string __repr__();

		Token get_var_name_tok();
		vector<Token> get_arg_name_toks();
		shared_ptr<ASTNode> get_body_node();
		bool isAnonymous();
		bool is_auto_return();

	private:
		Token var_name_tok; // 允许匿名函数，所以会给默认值
		vector<Token> arg_name_toks;
		shared_ptr<ASTNode> body_node;
		bool anonymous;
		bool auto_return; // 如果函数没有return语句则为auto return
	};

	// 函数唤醒结点
	class CallNode : public ASTNode
	{
	public:
		CallNode(shared_ptr<ASTNode> node_to_call, vector<shared_ptr<ASTNode>> arg_nodes);
		string __repr__();

		shared_ptr<ASTNode> get_func_node();
		vector<shared_ptr<ASTNode>> get_args_nodes();

	private:
		shared_ptr<ASTNode> func;
		vector<shared_ptr<ASTNode>> args;
	};

	class ReturnNode : public ASTNode
	{
	public:
		ReturnNode(shared_ptr<ASTNode> node_to_return, Position start = Position(), Position end = Position());
		string __repr__();

		shared_ptr<ASTNode> get_return_node();

	private:
		shared_ptr<ASTNode> node_to_return;
	};

	class ContinueNode : public ASTNode
	{
	public:
		ContinueNode(Position start, Position end);
		string __repr__();
	};

	class BreakNode : public ASTNode
	{
	public:
		BreakNode(Position start, Position end);
		string __repr__();
	};

}