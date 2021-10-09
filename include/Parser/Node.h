#pragma once
#include "Lexer/Token.h"
#include <memory>
#include <tuple>
#include <map>

using std::map;
using std::shared_ptr;
using std::tuple;

namespace Basic
{
	// 结点基类
	class ASTNode
	{
	public:
		virtual string repr() = 0;
		virtual ~ASTNode() = default;

	public:
		Position pos_start;
		Position pos_end;
	};

	// 数字结点
	class NumberNode : public ASTNode
	{
	public:
		NumberNode(const Token &tok);
		~NumberNode() = default;
		string repr();

		Token &get_tok();

	private:
		Token tok;
	};

	// 字符串结点
	class StringNode : public ASTNode
	{
	public:
		StringNode(const Token &tok);
		~StringNode() = default;
		string repr();

		Token &get_tok();

	private:
		Token tok;
	};

	// 列表结点
	class ListNode : public ASTNode
	{
	public:
		ListNode(const vector<shared_ptr<ASTNode>> &elem_nodes, const Position &start = Position(), const Position &end = Position());
		~ListNode() = default;

		const vector<shared_ptr<ASTNode>> &get_element_nodes();
		string repr();

	private:
		vector<shared_ptr<ASTNode>> element_nodes;
	};

	// 词典结点
	class DictNode : public ASTNode
	{
	public:
		DictNode(const map<string, shared_ptr<ASTNode>> &elem, const Position &start = Position(), const Position &end = Position());
		~DictNode() = default;

		const map<string, shared_ptr<ASTNode>> &get_elements();

		string repr();

	private:
		map<string, shared_ptr<ASTNode>> elements;
	};

	// 索引节点，e.g. list[1]
	class IndexNode : public ASTNode
	{
	public:
		IndexNode(const shared_ptr<ASTNode> &value, const shared_ptr<ASTNode> &index);
		~IndexNode() = default;

		const shared_ptr<ASTNode> &get_value();
		const shared_ptr<ASTNode> &get_index();
		string repr();

	private:
		shared_ptr<ASTNode> value;
		shared_ptr<ASTNode> index;
	};

	class AttrNode : public ASTNode
	{
	public:
		AttrNode(const shared_ptr<ASTNode> &elem, const Token &attr);
		~AttrNode() = default;

		const shared_ptr<ASTNode> &get_elem();
		const Token &get_attr();
		string repr();

	private:
		shared_ptr<ASTNode> elem;
		Token attr;
	};

	// 二元运算结点
	class BinOpNode : public ASTNode
	{
	public:
		BinOpNode(const shared_ptr<ASTNode> &_left, const Token &_op, const shared_ptr<ASTNode> &_right);
		~BinOpNode() = default;
		string repr();

		const shared_ptr<ASTNode> &get_left();
		const shared_ptr<ASTNode> &get_right();
		Token &get_op();

	private:
		Token op;
		shared_ptr<ASTNode> left;
		shared_ptr<ASTNode> right;
	};

	// 一元运算结点
	class UnaryOpNode : public ASTNode
	{
	public:
		UnaryOpNode(const Token &_op, const shared_ptr<ASTNode> &_node);
		~UnaryOpNode() = default;
		string repr();

		Token &get_op();
		const shared_ptr<ASTNode> &get_node();

	private:
		Token op;
		shared_ptr<ASTNode> node;
	};

	// 访问变量结点
	class VarAccessNode : public ASTNode
	{
	public:
		VarAccessNode(const Token &var_name_tok);
		~VarAccessNode() = default;
		string repr();

		Token &get_var_name_tok();

	private:
		Token var_name_tok;
	};

	class VarReferenceNode : public ASTNode
	{
	public:
		VarReferenceNode(const shared_ptr<ASTNode> &var);
		~VarReferenceNode() = default;
		string repr();

		const shared_ptr<ASTNode> &get_variable();

	private:
		shared_ptr<ASTNode> variable;
	};

	// 可变值节点（不同于Assign，这将直接修改地址指向的值）
	class MutateNode : public ASTNode
	{
	public:
		MutateNode(const shared_ptr<ASTNode> &mutant, const shared_ptr<ASTNode> &value);
		~MutateNode() = default;

		string repr();

		const shared_ptr<ASTNode> &get_mutant();
		const shared_ptr<ASTNode> &get_value();

	private:
		shared_ptr<ASTNode> mutant; // 要改变的变量
		shared_ptr<ASTNode> value;	// 给定的新值
	};

	class DefineNode : public ASTNode
	{
	public:
		DefineNode(const Token &var_name_tok, const shared_ptr<ASTNode> &value_node);
		~DefineNode() = default;
		string repr();

		Token &get_var_name_tok();
		const shared_ptr<ASTNode> &get_value_node();

	private:
		Token var_name_tok;
		shared_ptr<ASTNode> value_node;
	};

	// 变量赋值结点
	class VarAssignNode : public ASTNode
	{
	public:
		VarAssignNode(const vector<shared_ptr<ASTNode>> &assignments, const Position &start = Position(), const Position &end = Position());
		~VarAssignNode() = default;
		string repr();

		const vector<shared_ptr<ASTNode>> &get_assignments();

	private:
		vector<shared_ptr<ASTNode>> assignments;
	};

	// 删除变量结点
	class VarDeleteNode : public ASTNode
	{
	public:
		VarDeleteNode(const vector<Token> &deletion, const Position &start = Position(), const Position &end = Position());
		~VarDeleteNode() = default;
		string repr();

		const vector<Token> &get_deletion();

	private:
		vector<Token> deletion;
	};

	using Cases = vector<tuple<shared_ptr<ASTNode>, shared_ptr<ASTNode>, bool>>;
	using Else_Case = tuple<shared_ptr<ASTNode>, bool>;

	// If结点
	class IfNode : public ASTNode
	{
	public:
		IfNode(const Cases &_cases, const Else_Case &_else_case);
		~IfNode() = default;
		string repr();

		Cases &get_cases();
		Else_Case &get_else_case();

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
		ForNode(const Token &var_name, const shared_ptr<ASTNode> &start_value_node, const shared_ptr<ASTNode> &end_value_node, const shared_ptr<ASTNode> &body_node, const shared_ptr<ASTNode> &step_value_node = nullptr, bool return_null = false);
		~ForNode() = default;
		string repr();

		Token &get_var_name_tok();
		const shared_ptr<ASTNode> &get_start_value_node();
		const shared_ptr<ASTNode> &get_end_value_node();
		const shared_ptr<ASTNode> &get_body_node();
		const shared_ptr<ASTNode> &get_step_value_node();
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
		WhileNode(const shared_ptr<ASTNode> &condition, const shared_ptr<ASTNode> &body_node, bool return_null = false);
		~WhileNode() = default;
		string repr();

		const shared_ptr<ASTNode> &get_condition_node();
		const shared_ptr<ASTNode> &get_body_node();
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
		FuncDefNode(const Token &var_name, const vector<Token> &arg_name_toks, const shared_ptr<ASTNode> &body_node, bool anonymous = false, bool auto_return = true);
		~FuncDefNode() = default;
		string repr();

		Token &get_var_name_tok();
		const vector<Token> &get_arg_name_toks();
		const shared_ptr<ASTNode> &get_body_node();
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
		CallNode(const shared_ptr<ASTNode> &node_to_call, const vector<shared_ptr<ASTNode>> &arg_nodes);
		~CallNode() = default;
		string repr();

		const shared_ptr<ASTNode> &get_func_node();
		const vector<shared_ptr<ASTNode>> &get_args_nodes();

	private:
		shared_ptr<ASTNode> func;
		vector<shared_ptr<ASTNode>> args;
	};

	class ReturnNode : public ASTNode
	{
	public:
		ReturnNode(const shared_ptr<ASTNode> &node_to_return, const Position &start = Position(), const Position &end = Position());
		ReturnNode() = default;
		string repr();

		const shared_ptr<ASTNode> &get_return_node();

	private:
		shared_ptr<ASTNode> node_to_return;
	};

	class ContinueNode : public ASTNode
	{
	public:
		ContinueNode(const Position &start, const Position &end);
		~ContinueNode() = default;
		string repr();
	};

	class BreakNode : public ASTNode
	{
	public:
		BreakNode(const Position &start, const Position &end);
		~BreakNode() = default;
		string repr();
	};
}