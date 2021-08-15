#include "../../include/Parser/Node.h"
#include "../../include/Common/utils.h"

namespace Basic
{

    NumberNode::NumberNode(Token tok)
    {
        this->tok = tok;

        this->pos_start = tok.pos_start;
        this->pos_end = tok.pos_end;
    }

    string NumberNode::__repr__()
    {
        return tok.__repr__();
    }

    Token NumberNode::get_tok()
    {
        return this->tok;
    }

    StringNode::StringNode(Token tok)
    {
        this->tok = tok;

        this->pos_start = tok.pos_start;
        this->pos_end = tok.pos_end;
    }

    string StringNode::__repr__()
    {
        return tok.__repr__();
    }

    Token StringNode::get_tok()
    {
        return this->tok;
    }

    BinOpNode::BinOpNode(shared_ptr<ASTNode> _left, Token _op, shared_ptr<ASTNode> _right)
    {
        this->left = _left;
        this->op = _op;
        this->right = _right;

        this->pos_start = _left->pos_start;
        this->pos_end = _right->pos_end;
    }

    string BinOpNode::__repr__()
    {
        return Basic::format("(%s, %s, %s)", left->__repr__().c_str(), op.__repr__().c_str(), right->__repr__().c_str());
    }

    shared_ptr<ASTNode> BinOpNode::get_left()
    {
        return this->left;
    }

    shared_ptr<ASTNode> BinOpNode::get_right()
    {
        return this->right;
    }

    Token BinOpNode::get_op()
    {
        return this->op;
    }

    UnaryOpNode::UnaryOpNode(Token _op, shared_ptr<ASTNode> _node)
    {
        this->op = _op;
        this->node = _node;

        this->pos_start = _op.pos_start;
        this->pos_end = _node->pos_end;
    }

    string UnaryOpNode::__repr__()
    {
        return Basic::format("(%s, %s)", op.__repr__().c_str(), node->__repr__().c_str());
    }

    Token UnaryOpNode::get_op()
    {
        return this->op;
    }

    shared_ptr<ASTNode> UnaryOpNode::get_node()
    {
        return this->node;
    }

    VarAccessNode::VarAccessNode(Token var_name_tok)
    {
        this->var_name_tok = var_name_tok;
        this->pos_start = var_name_tok.pos_start;
        this->pos_end = var_name_tok.pos_end;
    }

    string VarAccessNode::__repr__()
    {
        return this->var_name_tok.__repr__();
    }

    Token VarAccessNode::get_var_name_tok()
    {
        return this->var_name_tok;
    }

    MutateNode::MutateNode(shared_ptr<ASTNode> mutant, shared_ptr<ASTNode> value)
    {
        this->mutant = mutant;
        this->value = value;

        this->pos_start = mutant->pos_start;
        this->pos_end = value->pos_end;
    }

    string MutateNode::__repr__()
    {
        return Basic::format("%s = %s",mutant->__repr__().c_str(),value->__repr__().c_str());
    }

    shared_ptr<ASTNode> MutateNode::get_mutant()
    {
        return this->mutant;
    }

    shared_ptr<ASTNode> MutateNode::get_value()
    {
        return this->value;
    }

    VarAssignNode::VarAssignNode(Token var_name_tok, shared_ptr<ASTNode> value_node)
    {
        this->var_name_tok = var_name_tok;
        this->value_node = value_node;
        this->pos_start = var_name_tok.pos_start;
        this->pos_end = value_node->pos_end;
    }

    string VarAssignNode::__repr__()
    {
        if (value_node != nullptr)
            return Basic::format("VAR %s = %s", this->var_name_tok.__repr__().c_str(), this->value_node->__repr__().c_str());
        else
            return Basic::format("VAR %s", this->var_name_tok.__repr__().c_str());
    }

    Token VarAssignNode::get_var_name_tok()
    {
        return this->var_name_tok;
    }

    shared_ptr<ASTNode> VarAssignNode::get_value_node()
    {
        return this->value_node;
    }

    IfNode::IfNode(Cases& _cases, Else_Case& _else_case)
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

    string IfNode::__repr__()
    {
        string result = "";

        for (auto& elem : cases)
        {
            result += "IF ";
            result += std::get<0>(elem)->__repr__();
            result += " THEN ";
            result += std::get<1>(elem)->__repr__();
            result += " ";
        }

        shared_ptr<ASTNode> else_node = std::get<0>(else_case);
        if (else_node != nullptr)
        {
            result += "ELSE ";
            result += else_node->__repr__();
        }

        return result;
    }

    Cases& IfNode::get_cases()
    {
        return this->cases;
    }

    Else_Case& IfNode::get_else_case()
    {
        return this->else_case;
    }

    ForNode::ForNode(Token var_name, shared_ptr<ASTNode> start_value_node, shared_ptr<ASTNode> end_value_node, shared_ptr<ASTNode> body_node, shared_ptr<ASTNode> step_value_node, bool return_null)
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

    string ForNode::__repr__()
    {
        string result = Basic::format("FOR %s = %s TO %s ", var_name_tok.__repr__().c_str(), start_value_node->__repr__().c_str(), end_value_node->__repr__().c_str());

        if (this->step_value_node != nullptr)
        {
            result += "STEP ";
            result += step_value_node->__repr__();
            result += " ";
        }

        result += "THEN ";
        result += body_node->__repr__();

        return result;
    }

    Token ForNode::get_var_name_tok()
    {
        return this->var_name_tok;
    }

    shared_ptr<ASTNode> ForNode::get_start_value_node()
    {
        return this->start_value_node;
    }

    shared_ptr<ASTNode> ForNode::get_end_value_node()
    {
        return this->end_value_node;
    }

    shared_ptr<ASTNode> ForNode::get_body_node()
    {
        return this->body_node;
    }

    shared_ptr<ASTNode> ForNode::get_step_value_node()
    {
        return this->step_value_node;
    }

    bool ForNode::is_return_null()
    {
        return this->return_null;
    }

    WhileNode::WhileNode(shared_ptr<ASTNode> condition, shared_ptr<ASTNode> body_node, bool return_null)
    {
        this->condition_node = condition;
        this->body_node = body_node;
        this->return_null = return_null;

        this->pos_start = condition->pos_start;
        this->pos_end = body_node->pos_end;
    }

    string WhileNode::__repr__()
    {
        return Basic::format("WHILE %s THEN %s", condition_node->__repr__().c_str(), body_node->__repr__().c_str());
    }

    shared_ptr<ASTNode> WhileNode::get_condition_node()
    {
        return this->condition_node;
    }

    shared_ptr<ASTNode> WhileNode::get_body_node()
    {
        return this->body_node;
    }

    bool WhileNode::is_return_null()
    {
        return this->return_null;
    }

    FuncDefNode::FuncDefNode(Token var_name, vector<Token> arg_name_toks, shared_ptr<ASTNode> body_node, bool anonymous, bool auto_return)
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

    string FuncDefNode::__repr__()
    {
        string result = "FUNC ";
        if (anonymous)
            result += "<anonymous>";
        else
            result += var_name_tok.value;

        result += "(";

        for (Token arg : arg_name_toks)
        {
            result += arg.__repr__();
            result += ",";
        }

        result.pop_back(); // 去除多余的,
        result += ") -> ";
        result += body_node->__repr__();

        return result;
    }

    Token FuncDefNode::get_var_name_tok()
    {
        return this->var_name_tok;
    }

    vector<Token> FuncDefNode::get_arg_name_toks()
    {
        return this->arg_name_toks;
    }

    shared_ptr<ASTNode> FuncDefNode::get_body_node()
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

    CallNode::CallNode(shared_ptr<ASTNode> node_to_call, vector<shared_ptr<ASTNode>> arg_nodes)
    {
        this->func = node_to_call;
        this->args = arg_nodes;

        this->pos_start = node_to_call->pos_start;

        if (!arg_nodes.empty())
            this->pos_end = arg_nodes.back()->pos_end;
        else
            this->pos_end = node_to_call->pos_end;
    }

    string CallNode::__repr__()
    {
        string result;

        result += func->__repr__();
        result += "(";

        for (auto arg : args)
        {
            result += arg->__repr__();
            result += ",";
        }
        result.pop_back();
        result += ")";

        return result;
    }

    shared_ptr<ASTNode> CallNode::get_func_node()
    {
        return this->func;
    }

    vector<shared_ptr<ASTNode>> CallNode::get_args_nodes()
    {
        return this->args;
    }

    ListNode::ListNode(vector<shared_ptr<ASTNode>>& elem_nodes, Position start, Position end)
    {
        this->element_nodes = elem_nodes;
        this->pos_start = start;
        this->pos_end = end;
    }

    vector<shared_ptr<ASTNode>> ListNode::get_element_nodes()
    {
        return this->element_nodes;
    }

    string ListNode::__repr__()
    {
        string result = "[";
        for (auto node : element_nodes)
        {
            result += node->__repr__();
            result.push_back(',');
        }

        if (result != "[")
            result.pop_back();
        result.push_back(']');

        return result;
    }

    IndexNode::IndexNode(shared_ptr<ASTNode> value, shared_ptr<ASTNode> index)
    {
        this->value = value;
        this->index = index;

        this->pos_start = value->pos_start;
        this->pos_end = index->pos_end;
    }

    shared_ptr<ASTNode> IndexNode::get_value()
    {
        return this->value;
    }

    shared_ptr<ASTNode> IndexNode::get_index()
    {
        return this->index;
    }

    string IndexNode::__repr__()
    {
        return Basic::format("%s[%s]", value->__repr__().c_str(), index->__repr__().c_str());
    }

    ReturnNode::ReturnNode(shared_ptr<ASTNode> node_to_return, Position start, Position end)
    {
        this->node_to_return = node_to_return;
        this->pos_start = start;
        this->pos_end = end;
    }

    string ReturnNode::__repr__()
    {
        return Basic::format("RETURN %s", node_to_return->__repr__().c_str());
    }

    shared_ptr<ASTNode> ReturnNode::get_return_node()
    {
        return this->node_to_return;
    }

    ContinueNode::ContinueNode(Position start, Position end)
    {
        this->pos_start = start;
        this->pos_end = end;
    }

    string ContinueNode::__repr__()
    {
        return "CONTINUE";
    }

    BreakNode::BreakNode(Position start, Position end)
    {
        this->pos_start = start;
        this->pos_end = end;
    }

    string BreakNode::__repr__()
    {
        return "BREAK";
    }

}