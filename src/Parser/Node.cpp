#include "Parser/Node.h"
#include "Common/utils.h"

namespace Basic
{
	NumberNode::NumberNode(const Token &tok)
	{
		this->tok = tok;

		this->pos_start = tok.pos_start;
		this->pos_end = tok.pos_end;
	}

	string NumberNode::repr()
	{
		return tok.repr();
	}

	Token &NumberNode::get_tok()
	{
		return this->tok;
	}

	StringNode::StringNode(const Token &tok)
	{
		this->tok = tok;

		this->pos_start = tok.pos_start;
		this->pos_end = tok.pos_end;
	}

	string StringNode::repr()
	{
		return tok.repr();
	}

	Token &StringNode::get_tok()
	{
		return this->tok;
	}

	BinOpNode::BinOpNode(const shared_ptr<ASTNode> &_left, const Token &_op, const shared_ptr<ASTNode> &_right)
	{
		this->left = _left;
		this->op = _op;
		this->right = _right;

		this->pos_start = _left->pos_start;
		this->pos_end = _right->pos_end;
	}

	string BinOpNode::repr()
	{
		return Basic::format("(%s, %s, %s)", left->repr().c_str(), op.repr().c_str(), right->repr().c_str());
	}

	const shared_ptr<ASTNode> &BinOpNode::get_left()
	{
		return this->left;
	}

	const shared_ptr<ASTNode> &BinOpNode::get_right()
	{
		return this->right;
	}

	Token &BinOpNode::get_op()
	{
		return this->op;
	}

	UnaryOpNode::UnaryOpNode(const Token &_op, const shared_ptr<ASTNode> &_node)
	{
		this->op = _op;
		this->node = _node;

		this->pos_start = _op.pos_start;
		this->pos_end = _node->pos_end;
	}

	string UnaryOpNode::repr()
	{
		return Basic::format("(%s, %s)", op.repr().c_str(), node->repr().c_str());
	}

	Token &UnaryOpNode::get_op()
	{
		return this->op;
	}

	const shared_ptr<ASTNode> &UnaryOpNode::get_node()
	{
		return this->node;
	}

	VarAccessNode::VarAccessNode(const Token &var_name_tok)
	{
		this->var_name_tok = var_name_tok;
		this->pos_start = var_name_tok.pos_start;
		this->pos_end = var_name_tok.pos_end;
	}

	string VarAccessNode::repr()
	{
		return this->var_name_tok.repr();
	}

	Token &VarAccessNode::get_var_name_tok()
	{
		return this->var_name_tok;
	}

	VarReferenceNode::VarReferenceNode(const shared_ptr<ASTNode> &var)
	{
		this->variable = var;
		this->pos_start = var->pos_start;
		this->pos_end = var->pos_end;
	}

	string VarReferenceNode::repr()
	{
		return "&" + this->variable->repr();
	}

	const shared_ptr<ASTNode> &VarReferenceNode::get_variable()
	{
		return this->variable;
	}

	MutateNode::MutateNode(const shared_ptr<ASTNode> &mutant, const shared_ptr<ASTNode> &value)
	{
		this->mutant = mutant;
		this->value = value;

		this->pos_start = mutant->pos_start;
		this->pos_end = value->pos_end;
	}

	string MutateNode::repr()
	{
		return Basic::format("%s = %s", mutant->repr().c_str(), value->repr().c_str());
	}

	const shared_ptr<ASTNode> &MutateNode::get_mutant()
	{
		return this->mutant;
	}

	const shared_ptr<ASTNode> &MutateNode::get_value()
	{
		return this->value;
	}

	DefineNode::DefineNode(const Token &var_name_tok, const shared_ptr<ASTNode> &value_node)
	{
		this->var_name_tok = var_name_tok;
		this->value_node = value_node;
		this->pos_start = var_name_tok.pos_start;
		this->pos_end = value_node->pos_end;
	}

	string DefineNode::repr()
	{
		if (value_node != nullptr)
			return Basic::format("VAR %s = %s", this->var_name_tok.repr().c_str(), this->value_node->repr().c_str());
		else
			return Basic::format("VAR %s", this->var_name_tok.repr().c_str());
	}

	Token &DefineNode::get_var_name_tok()
	{
		return this->var_name_tok;
	}

	const shared_ptr<ASTNode> &DefineNode::get_value_node()
	{
		return this->value_node;
	}

	VarAssignNode::VarAssignNode(const vector<shared_ptr<ASTNode>>& assignments, const Position& start, const Position& end)
	{
		this->assignments = assignments;
		this->pos_start = start;
		this->pos_end = end;
	}

	string VarAssignNode::repr()
	{
		string result = "";
		for (auto const& ptr : assignments)
		{
			result += ptr->repr();
			result.push_back(',');
		}

		if (result != "")
			result.pop_back();

		return result;
	}

	const vector<shared_ptr<ASTNode>>& VarAssignNode::get_assignments()
	{
		return this->assignments;
	}

	VarDeleteNode::VarDeleteNode(const vector<Token>& deletion, const Position& start, const Position& end)
	{
		this->deletion = deletion;
		this->pos_start = start;
		this->pos_end = end;
	}

	string VarDeleteNode::repr()
	{
		string result = "DEL ";
		for (auto const& tok : deletion)
		{
			result += tok.value;
			result.push_back(',');
		}
		result.pop_back();

		return result;
	}

	const vector<Token>& VarDeleteNode::get_deletion()
	{
		return this->deletion;
	}

	IfNode::IfNode(const Cases &_cases, const Else_Case &_else_case)
	{
		this->cases = _cases;
		this->else_case = _else_case;

		if (!_cases.empty())
		{
			this->pos_start = std::get<0>(_cases[0])->pos_start;

			shared_ptr<ASTNode> else_node = std::get<0>(_else_case);
			if (else_node != nullptr)
			{
				this->pos_end = else_node->pos_end;
			}
			else
			{
				this->pos_end = std::get<0>(_cases.back())->pos_end;
			}
		}
	}

	string IfNode::repr()
	{
		string result = "";

		for (auto &elem : cases)
		{
			result += "IF ";
			result += std::get<0>(elem)->repr();
			result += " THEN ";
			result += std::get<1>(elem)->repr();
			result += " ";
		}

		shared_ptr<ASTNode> else_node = std::get<0>(else_case);
		if (else_node != nullptr)
		{
			result += "ELSE ";
			result += else_node->repr();
		}

		return result;
	}

	Cases &IfNode::get_cases()
	{
		return this->cases;
	}

	Else_Case &IfNode::get_else_case()
	{
		return this->else_case;
	}

	ForNode::ForNode(const Token &var_name, const shared_ptr<ASTNode> &start_value_node, const shared_ptr<ASTNode> &end_value_node, const shared_ptr<ASTNode> &body_node, const shared_ptr<ASTNode> &step_value_node, bool return_null)
	{
		this->var_name_tok = var_name;
		this->start_value_node = start_value_node;
		this->end_value_node = end_value_node;
		this->body_node = body_node;
		this->step_value_node = step_value_node;
		this->return_null = return_null;

		this->pos_start = var_name.pos_start;
		this->pos_end = body_node->pos_end;
	}

	string ForNode::repr()
	{
		string result = Basic::format("FOR %s = %s TO %s ", var_name_tok.repr().c_str(), start_value_node->repr().c_str(), end_value_node->repr().c_str());

		if (this->step_value_node != nullptr)
		{
			result += "STEP ";
			result += step_value_node->repr();
			result += " ";
		}

		result += "THEN ";
		result += body_node->repr();

		return result;
	}

	Token &ForNode::get_var_name_tok()
	{
		return this->var_name_tok;
	}

	const shared_ptr<ASTNode> &ForNode::get_start_value_node()
	{
		return this->start_value_node;
	}

	const shared_ptr<ASTNode> &ForNode::get_end_value_node()
	{
		return this->end_value_node;
	}

	const shared_ptr<ASTNode> &ForNode::get_body_node()
	{
		return this->body_node;
	}

	const shared_ptr<ASTNode> &ForNode::get_step_value_node()
	{
		return this->step_value_node;
	}

	bool ForNode::is_return_null()
	{
		return this->return_null;
	}

	WhileNode::WhileNode(const shared_ptr<ASTNode> &condition, const shared_ptr<ASTNode> &body_node, bool return_null)
	{
		this->condition_node = condition;
		this->body_node = body_node;
		this->return_null = return_null;

		this->pos_start = condition->pos_start;
		this->pos_end = body_node->pos_end;
	}

	string WhileNode::repr()
	{
		return Basic::format("WHILE %s THEN %s", condition_node->repr().c_str(), body_node->repr().c_str());
	}

	const shared_ptr<ASTNode> &WhileNode::get_condition_node()
	{
		return this->condition_node;
	}

	const shared_ptr<ASTNode> &WhileNode::get_body_node()
	{
		return this->body_node;
	}

	bool WhileNode::is_return_null()
	{
		return this->return_null;
	}

	FuncDefNode::FuncDefNode(const Token &var_name, const vector<Token> &arg_name_toks, const shared_ptr<ASTNode> &body_node, bool anonymous, bool auto_return)
	{
		this->var_name_tok = var_name;
		this->arg_name_toks = arg_name_toks;
		this->body_node = body_node;
		this->anonymous = anonymous;
		this->auto_return = auto_return;

		if (!anonymous)
		{
			this->pos_start = var_name.pos_start;
		}
		else if (!arg_name_toks.empty())
		{
			this->pos_start = var_name.pos_start;
		}
		else
			this->pos_start = body_node->pos_start;

		this->pos_end = body_node->pos_end;
	}

	string FuncDefNode::repr()
	{
		string result = "FUNC ";
		if (anonymous)
			result += "<anonymous>";
		else
			result += var_name_tok.value;

		result += "(";

		for (Token arg : arg_name_toks)
		{
			result += arg.repr();
			result += ",";
		}

		if (result.back() != '(')
			result.pop_back(); // 去除多余的,

		result += ") -> ";
		result += body_node->repr();

		return result;
	}

	Token &FuncDefNode::get_var_name_tok()
	{
		return this->var_name_tok;
	}

	const vector<Token> &FuncDefNode::get_arg_name_toks()
	{
		return this->arg_name_toks;
	}

	const shared_ptr<ASTNode> &FuncDefNode::get_body_node()
	{
		return this->body_node;
	}

	bool FuncDefNode::isAnonymous()
	{
		return this->anonymous;
	}

	bool FuncDefNode::is_auto_return()
	{
		return this->auto_return;
	}

	CallNode::CallNode(const shared_ptr<ASTNode> &node_to_call, const vector<shared_ptr<ASTNode>> &arg_nodes)
	{
		this->func = node_to_call;
		this->args = arg_nodes;

		this->pos_start = node_to_call->pos_start;

		if (!arg_nodes.empty())
			this->pos_end = arg_nodes.back()->pos_end;
		else
			this->pos_end = node_to_call->pos_end;
	}

	string CallNode::repr()
	{
		string result;

		result += func->repr();
		result += "(";

		for (auto arg : args)
		{
			result += arg->repr();
			result += ",";
		}

		if (result.back() != '(')
			result.pop_back();

		result += ")";

		return result;
	}

	const shared_ptr<ASTNode> &CallNode::get_func_node()
	{
		return this->func;
	}

	const vector<shared_ptr<ASTNode>> &CallNode::get_args_nodes()
	{
		return this->args;
	}

	ListNode::ListNode(const vector<shared_ptr<ASTNode>> &elem_nodes, const Position &start, const Position &end)
	{
		this->element_nodes = elem_nodes;
		this->pos_start = start;
		this->pos_end = end;
	}

	const vector<shared_ptr<ASTNode>> &ListNode::get_element_nodes()
	{
		return this->element_nodes;
	}

	string ListNode::repr()
	{
		string result = "[";
		for (auto node : element_nodes)
		{
			result += node->repr();
			result.push_back(',');
		}

		if (result != "[")
			result.pop_back();
		result.push_back(']');

		return result;
	}

	DictNode::DictNode(const map<string, shared_ptr<ASTNode>> &elem, const Position &start, const Position &end)
	{
		this->elements = elem;
		this->pos_start = start;
		this->pos_end = end;
	}

	const map<string, shared_ptr<ASTNode>> &DictNode::get_elements()
	{
		return this->elements;
	}

	string DictNode::repr()
	{
		string result = "{";
		for (auto node : elements)
		{
			result += node.first;
			result.push_back(':');
			result += node.second->repr();
			result.push_back(',');
		}

		if (result != "{")
			result.pop_back();
		result.push_back('}');

		return result;
	}

	IndexNode::IndexNode(const shared_ptr<ASTNode> &value, const shared_ptr<ASTNode> &index)
	{
		this->value = value;
		this->index = index;

		this->pos_start = value->pos_start;
		this->pos_end = index->pos_end;
	}

	const shared_ptr<ASTNode> &IndexNode::get_value()
	{
		return this->value;
	}

	const shared_ptr<ASTNode> &IndexNode::get_index()
	{
		return this->index;
	}

	string IndexNode::repr()
	{
		return Basic::format("%s[%s]", value->repr().c_str(), index->repr().c_str());
	}

	AttrNode::AttrNode(const shared_ptr<ASTNode> &elem, const Token &attr)
	{
		this->elem = elem;
		this->attr = attr;

		this->pos_start = elem->pos_start;
		this->pos_end = attr.pos_end;
	}

	const shared_ptr<ASTNode> &AttrNode::get_elem()
	{
		return this->elem;
	}

	const Token &AttrNode::get_attr()
	{
		return this->attr;
	}

	string AttrNode::repr()
	{
		return this->elem->repr() + "." + this->attr.repr();
	}

	ReturnNode::ReturnNode(const shared_ptr<ASTNode> &node_to_return, const Position &start, const Position &end)
	{
		this->node_to_return = node_to_return;
		this->pos_start = start;
		this->pos_end = end;
	}

	string ReturnNode::repr()
	{
		return Basic::format("RETURN %s", node_to_return->repr().c_str());
	}

	const shared_ptr<ASTNode> &ReturnNode::get_return_node()
	{
		return this->node_to_return;
	}

	ContinueNode::ContinueNode(const Position &start, const Position &end)
	{
		this->pos_start = start;
		this->pos_end = end;
	}

	string ContinueNode::repr()
	{
		return "CONTINUE";
	}

	BreakNode::BreakNode(const Position &start, const Position &end)
	{
		this->pos_start = start;
		this->pos_end = end;
	}

	string BreakNode::repr()
	{
		return "BREAK";
	}

}