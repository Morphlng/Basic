#include "../../include/Interpreter/Interpreter.h"
#include "../../include/Common/utils.h"
#include "../../include/Interpreter/RunTimeError.h"
#include "../../include/Interpreter/Data.h"
#include <functional>

namespace Basic
{
	RuntimeResult Interpreter::visit(ASTNode *root, Context &context, bool byRef)
	{
		if (typeid(*root) == typeid(NumberNode))
		{
			return visit_NumberNode(static_cast<NumberNode *>(root), context);
		}
		else if (typeid(*root) == typeid(StringNode))
		{
			return visit_StringNode(static_cast<StringNode *>(root), context);
		}
		else if (typeid(*root) == typeid(ListNode))
		{
			return visit_ListNode(static_cast<ListNode *>(root), context);
		}
		else if (typeid(*root) == typeid(IndexNode))
		{
			return visit_IndexNode(static_cast<IndexNode *>(root), context);
		}
		else if (typeid(*root) == typeid(BinOpNode))
		{
			return visit_BinOpNode(static_cast<BinOpNode *>(root), context);
		}
		else if (typeid(*root) == typeid(UnaryOpNode))
		{
			return visit_UnaryOpNode(static_cast<UnaryOpNode *>(root), context);
		}
		else if (typeid(*root) == typeid(VarAccessNode))
		{
			return visit_VarAccessNode(static_cast<VarAccessNode *>(root), context, byRef);
		}
		else if (typeid(*root) == typeid(VarReferenceNode))
		{
			VarReferenceNode *refNode = static_cast<VarReferenceNode *>(root);
			return visit(refNode->get_variable().get(), context, true);
		}
		else if (typeid(*root) == typeid(VarAssignNode))
		{
			return visit_VarAssignNode(static_cast<VarAssignNode *>(root), context);
		}
		else if (typeid(*root) == typeid(MutateNode))
		{
			return visit_MutateNode(static_cast<MutateNode *>(root), context);
		}
		else if (typeid(*root) == typeid(IfNode))
		{
			return visit_IfNode(static_cast<IfNode *>(root), context);
		}
		else if (typeid(*root) == typeid(ForNode))
		{
			return visit_ForNode(static_cast<ForNode *>(root), context);
		}
		else if (typeid(*root) == typeid(WhileNode))
		{
			return visit_WhileNode(static_cast<WhileNode *>(root), context);
		}
		else if (typeid(*root) == typeid(FuncDefNode))
		{
			return visit_FuncDefNode(static_cast<FuncDefNode *>(root), context);
		}
		else if (typeid(*root) == typeid(CallNode))
		{
			return visit_CallNode(static_cast<CallNode *>(root), context);
		}
		else if (typeid(*root) == typeid(ReturnNode))
		{
			return visit_ReturnNode(static_cast<ReturnNode *>(root), context);
		}
		else if (typeid(*root) == typeid(BreakNode))
		{
			return visit_BreakNode(static_cast<BreakNode *>(root), context);
		}
		else if (typeid(*root) == typeid(ContinueNode))
		{
			return visit_ContinueNode(static_cast<ContinueNode *>(root), context);
		}
		else
		{
			string message = Basic::format("No visit method for %s defined", typeid(*root).name());

			return RuntimeResult().failure(make_shared<RunTimeError>(root->pos_start, root->pos_end, message, context));
		}
	}

	RuntimeResult Interpreter::visit_NumberNode(NumberNode *root, Context &context)
	{
		RuntimeResult res;
		Number num(root->get_tok().get_number(), root->pos_start, root->pos_end);
		num.set_context(&context);

		return res.success(make_Dataptr<Number>(num));
	}

	RuntimeResult Interpreter::visit_StringNode(StringNode *root, Context &context)
	{
		RuntimeResult res;

		String str(root->get_tok().value);
		str.set_pos(root->pos_start, root->pos_end);
		str.set_context(&context);

		return res.success(make_Dataptr<String>(str));
	}

	RuntimeResult Interpreter::visit_ListNode(ListNode *root, Context &context)
	{
		RuntimeResult res;
		vector<DataPtr> elements;

		for (auto const &elem_node : root->get_element_nodes())
		{
			DataPtr elem = res.registry(visit(elem_node.get(), context));
			if (res.should_return())
				return res;

			if (typeid(**elem) != typeid(Data))
				elements.push_back(elem);
		}

		List result(elements);
		result.set_context(&context);
		result.set_pos(root->pos_start, root->pos_end);

		return res.success(make_Dataptr<List>(result));
	}

	RuntimeResult Interpreter::visit_BinOpNode(BinOpNode *root, Context &context)
	{
		RuntimeResult res;

		const shared_ptr<ASTNode> &left_node = root->get_left();
		const shared_ptr<ASTNode> &right_node = root->get_right();

		DataPtr left = res.registry(visit(left_node.get(), context));
		if (res.should_return())
			return res;

		DataPtr right = res.registry(visit(right_node.get(), context));
		if (res.should_return())
			return res;

		DataPtr result;
		try
		{
			if (root->get_op().type == TD_PLUS)
			{
				result = (*left)->added_to(right);
			}
			else if (root->get_op().type == TD_MINUS)
			{
				result = (*left)->subbed_by(right);
			}
			else if (root->get_op().type == TD_MUL)
			{
				result = (*left)->multed_by(right);
			}
			else if (root->get_op().type == TD_DIV)
			{
				result = (*left)->dived_by(right);
			}
			else if (root->get_op().type == TD_POW)
			{
				result = (*left)->powed_by(right);
			}
			else if (root->get_op().type == TD_EE)
			{
				result = (*left)->get_comparison_eq(right);
			}
			else if (root->get_op().type == TD_NE)
			{
				result = (*left)->get_comparison_ne(right);
			}
			else if (root->get_op().type == TD_LT)
			{
				result = (*left)->get_comparison_lt(right);
			}
			else if (root->get_op().type == TD_GT)
			{
				result = (*left)->get_comparison_gt(right);
			}
			else if (root->get_op().type == TD_LTE)
			{
				result = (*left)->get_comparison_lte(right);
			}
			else if (root->get_op().type == TD_GTE)
			{
				result = (*left)->get_comparison_gte(right);
			}
			else if (root->get_op().matches(TD_KEYWORD, "AND"))
			{
				result = (*left)->anded_by(right);
			}
			else if (root->get_op().matches(TD_KEYWORD, "OR"))
			{
				result = (*left)->ored_by(right);
			}

			(*result)->set_pos(root->pos_start, root->pos_end);
			return res.success(result);
		}
		catch (RunTimeError &e)
		{
			return res.failure(make_shared<RunTimeError>(e));
		}
	}

	RuntimeResult Interpreter::visit_UnaryOpNode(UnaryOpNode *root, Context &context)
	{
		RuntimeResult res;
		DataPtr num = res.registry(visit(root->get_node().get(), context));
		if (res.should_return())
			return res;

		if (root->get_op().type == TD_MINUS)
			num = (*num)->multed_by(make_Dataptr<Number>(-1));
		else if (root->get_op().matches(TD_KEYWORD, "NOT"))
			num = (*num)->notted();

		(*num)->set_pos(root->pos_start, root->pos_end);

		return res.success(num);
	}

	RuntimeResult Interpreter::visit_VarAccessNode(VarAccessNode *root, Context &context, bool byRef)
	{
		RuntimeResult res;
		string var_name = root->get_var_name_tok().value;
		DataPtr value = context.get_symbol_table().get(var_name);

		if (value == nullptr)
			return res.failure(make_shared<RunTimeError>(root->pos_start, root->pos_end, var_name + " is not defined", context));

		// 多数情况，我们不想修改指针指向的数据，只想要一份值的拷贝
		// 但是有些时候又需要
		// 故该函数需要查看传入的byRef参数
		if (!byRef && typeid(**value) != typeid(List))
		{
			DataPtr copy_value = (*value)->clone();
			(*copy_value)->set_context(&context);
			(*copy_value)->set_pos(root->pos_start, root->pos_end);

			return res.success(copy_value);
		}

		(*value)->set_pos(root->pos_start, root->pos_end);
		(*value)->set_context(&context);

		return res.success(value);
	}

	RuntimeResult Interpreter::visit_VarAssignNode(VarAssignNode *root, Context &context)
	{
		RuntimeResult res;
		string var_name = root->get_var_name_tok().value;

		DataPtr value = res.registry(visit(root->get_value_node().get(), context));
		if (res.should_return())
			return res;

		context.get_symbol_table().set(var_name, (*value)->clone());

		return res.success(value);
	}

	RuntimeResult Interpreter::visit_MutateNode(MutateNode *root, Context &context)
	{
		RuntimeResult res;

		const shared_ptr<ASTNode> &mutant_node = root->get_mutant();
		const shared_ptr<ASTNode> &value_node = root->get_value();

		DataPtr mutant = res.registry(visit(mutant_node.get(), context, true));
		if (res.should_return())
			return res;

		DataPtr value = res.registry(visit(value_node.get(), context, true));
		if (res.should_return())
			return res;

		// 复制，然后通过移动，转让权限
		// 之所以复制，是为了保证：
		// VAR list[0] = list[1]这样的赋值不会导致list[1]失效
		*mutant = move(*(*value)->clone());

		return res.success(mutant);
	}

	RuntimeResult Interpreter::visit_IndexNode(IndexNode *root, Context &context)
	{
		RuntimeResult res;
		DataPtr value = res.registry(visit(root->get_value().get(), context, true));
		if (res.should_return())
			return res;

		if (typeid(**value) == typeid(List) || typeid(**value) == typeid(String))
		{
			try
			{
				DataPtr index = res.registry(visit(root->get_index().get(), context));
				DataPtr result = (*value)->index_by(index);
				(*result)->set_pos(root->pos_start, root->pos_end);
				(*result)->set_context(&context);

				return res.success(result);
			}
			catch (RunTimeError &e)
			{
				return res.failure(make_shared<RunTimeError>(e));
			}
		}
		else
			return res.failure(make_shared<RunTimeError>(root->pos_start, root->pos_end, "Can only index List and String", context));
	}

	RuntimeResult Interpreter::visit_IfNode(IfNode *root, Context &context)
	{
		RuntimeResult res;

		for (auto &elem : root->get_cases())
		{
			const shared_ptr<ASTNode> &condition_node = std::get<0>(elem);
			const shared_ptr<ASTNode> &expr_node = std::get<1>(elem);
			bool should_return_null = std::get<2>(elem);

			DataPtr condition = res.registry(visit(condition_node.get(), context));
			Number *condition_value = raw_Dataptr<Number>(condition);
			if (res.should_return())
				return res;

			if (condition_value->is_true())
			{
				DataPtr expr_value = res.registry(visit(expr_node.get(), context));
				if (res.should_return())
					return res;

				if (should_return_null)
					return res.success(make_Dataptr<Data>(Number::null));
				else
					return res.success(expr_value);
			}
		}

		Else_Case &else_case = root->get_else_case();
		const shared_ptr<ASTNode> &else_node = std::get<0>(else_case);
		if (else_node != nullptr)
		{
			DataPtr else_value = res.registry(visit(else_node.get(), context));
			if (res.should_return())
				return res;

			if (std::get<1>(else_case))
				return res.success(make_Dataptr<Data>(Number::null));
			else
				return res.success(else_value);
		}

		return res.success(make_Dataptr<Data>(Number::null));
	}

	RuntimeResult Interpreter::visit_ForNode(ForNode *root, Context &context)
	{
		RuntimeResult res;
		vector<DataPtr> elements;

		DataPtr start = res.registry(visit(root->get_start_value_node().get(), context));
		if (res.should_return())
			return res;
		Number *start_data = raw_Dataptr<Number>(start);

		DataPtr end = res.registry(visit(root->get_end_value_node().get(), context));
		if (res.should_return())
			return res;
		Number *end_data = raw_Dataptr<Number>(end);

		const shared_ptr<ASTNode> &step_node = root->get_step_value_node();

		int i = start_data->get_value(true);
		int end_value = end_data->get_value(true);

		int step_value = 1;
		if (step_node != nullptr)
		{
			DataPtr step = res.registry(visit(step_node.get(), context));
			if (res.should_return())
				return res;
			Number *step_data = raw_Dataptr<Number>(step);
			step_value = step_data->get_value();
		}

		std::function<bool(int, int)> f;

		// 步长可以是负的
		if (step_value >= 0)
			f = [=](int a, int b)
			{ return a <= b; };
		else
			f = [=](int a, int b)
			{ return a >= b; };

		while (f(i, end_value))
		{
			context.get_symbol_table().set(root->get_var_name_tok().value, make_Dataptr<Number>(i));
			i += step_value;

			DataPtr elem = res.registry(visit(root->get_body_node().get(), context));
			if (res.should_return() && !res.should_break() && !res.should_continue())
				return res;

			if (res.should_continue())
				continue;

			if (res.should_break())
				break;

			if (typeid(**elem) != typeid(Data))
				elements.push_back(elem);
		}

		if (root->is_return_null() || elements.empty())
			return res.success(make_Dataptr<Data>(Number::null));
		else
		{
			List result(elements);
			result.set_context(&context);
			result.set_pos(root->pos_start, root->pos_end);
			return res.success(make_Dataptr<List>(result));
		}
	}

	RuntimeResult Interpreter::visit_WhileNode(WhileNode *root, Context &context)
	{
		RuntimeResult res;
		vector<DataPtr> elements;

		while (true)
		{
			DataPtr cond = res.registry(visit(root->get_condition_node().get(), context));
			if (res.should_return())
				return res;
			Number *condition = raw_Dataptr<Number>(cond);

			if (!condition->is_true())
				break;

			DataPtr elem = res.registry(visit(root->get_body_node().get(), context));
			if (res.should_return() && !res.should_break() && !res.should_continue())
				return res;

			if (res.should_continue())
				continue;

			if (res.should_break())
				break;

			if (typeid(**elem) != typeid(Data))
				elements.push_back(elem);
		}

		if (root->is_return_null())
			return res.success(make_Dataptr<Data>(Number::null));
		else
		{
			if (!elements.empty())
			{
				List result(elements);
				result.set_context(&context);
				result.set_pos(root->pos_start, root->pos_end);
				return res.success(make_Dataptr<List>(result));
			}

			return res.success(make_Dataptr<Data>(Number::null));
		}
	}

	RuntimeResult Interpreter::visit_FuncDefNode(FuncDefNode *root, Context &context)
	{
		RuntimeResult res;

		string func_name = root->get_var_name_tok().value;
		const shared_ptr<ASTNode> &body_node = root->get_body_node();

		vector<string> arg_names;
		for (const Token &tok : root->get_arg_name_toks())
		{
			arg_names.push_back(tok.value);
		}

		DataPtr func = make_Dataptr<Function>(func_name, body_node, arg_names, root->is_auto_return());
		(*func)->set_pos(root->pos_start, root->pos_end);

		if (!root->isAnonymous())
		{
			context.get_symbol_table().set(func_name, func);
		}
		(*func)->set_context(&context);

		return res.success(func);
	}

	RuntimeResult Interpreter::visit_CallNode(CallNode *root, Context &context)
	{
		RuntimeResult res;
		vector<DataPtr> args;

		DataPtr value_to_call = res.registry(visit(root->get_func_node().get(), context));
		if (res.should_return())
			return res;

		(*value_to_call)->set_pos(root->pos_start, root->pos_end);

		for (auto const &arg_node : root->get_args_nodes())
		{
			args.push_back(res.registry(visit(arg_node.get(), context)));
			if (res.should_return())
				return res;
		}

		DataPtr return_value = res.registry((*value_to_call)->execute(args));
		if (res.should_return())
			return res;

		(*return_value)->set_pos(root->pos_start, root->pos_end);
		(*return_value)->set_context(&context);

		return res.success(return_value);
	}

	RuntimeResult Interpreter::visit_ReturnNode(ReturnNode *root, Context &context)
	{
		RuntimeResult res;

		const shared_ptr<ASTNode> &return_node = root->get_return_node();
		DataPtr return_value;
		if (return_node != nullptr)
		{
			return_value = res.registry(visit(return_node.get(), context));
			if (res.should_return())
				return res;
		}
		else
			return_value = make_Dataptr<Data>(Number::null);

		return res.success_return(return_value);
	}

	RuntimeResult Interpreter::visit_BreakNode(BreakNode *root, Context &context)
	{
		return RuntimeResult().success_break();
	}

	RuntimeResult Interpreter::visit_ContinueNode(ContinueNode *root, Context &context)
	{
		return RuntimeResult().success_continue();
	}
}