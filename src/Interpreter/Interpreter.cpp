#include "../../include/Interpreter/Interpreter.h"
#include "../../include/Common/utils.h"
#include "../../include/Interpreter/RunTimeError.h"
#include <stdexcept>
#include <functional>

namespace Basic {

	RuntimeResult Interpreter::visit(shared_ptr<ASTNode> root, Context& context)
	{
		if (typeid(*root) == typeid(NumberNode))
		{
			return visit_NumberNode(std::dynamic_pointer_cast<NumberNode>(root), context);
		}
		else if (typeid(*root) == typeid(StringNode))
		{
			return visit_StringNode(std::dynamic_pointer_cast<StringNode>(root), context);
		}
		else if (typeid(*root) == typeid(ListNode))
		{
			return visit_ListNode(std::dynamic_pointer_cast<ListNode>(root), context);
		}
		else if (typeid(*root) == typeid(IndexNode))
		{
			return visit_IndexNode(std::dynamic_pointer_cast<IndexNode>(root), context);
		}
		else if (typeid(*root) == typeid(BinOpNode))
		{
			return visit_BinOpNode(std::dynamic_pointer_cast<BinOpNode>(root), context);
		}
		else if (typeid(*root) == typeid(UnaryOpNode))
		{
			return visit_UnaryOpNode(std::dynamic_pointer_cast<UnaryOpNode>(root), context);
		}
		else if (typeid(*root) == typeid(VarAccessNode))
		{
			return visit_VarAccessNode(std::dynamic_pointer_cast<VarAccessNode>(root), context);
		}
		else if (typeid(*root) == typeid(VarAssignNode))
		{
			return visit_VarAssignNode(std::dynamic_pointer_cast<VarAssignNode>(root), context);
		}
		else if (typeid(*root) == typeid(MutateNode))
		{
			return visit_MutateNode(std::dynamic_pointer_cast<MutateNode>(root), context);
		}
		else if (typeid(*root) == typeid(IfNode))
		{
			return visit_IfNode(std::dynamic_pointer_cast<IfNode>(root), context);
		}
		else if (typeid(*root) == typeid(ForNode))
		{
			return visit_ForNode(std::dynamic_pointer_cast<ForNode>(root), context);
		}
		else if (typeid(*root) == typeid(WhileNode))
		{
			return visit_WhileNode(std::dynamic_pointer_cast<WhileNode>(root), context);
		}
		else if (typeid(*root) == typeid(FuncDefNode))
		{
			return visit_FuncDefNode(std::dynamic_pointer_cast<FuncDefNode>(root), context);
		}
		else if (typeid(*root) == typeid(CallNode))
		{
			return visit_CallNode(std::dynamic_pointer_cast<CallNode>(root), context);
		}
		else if (typeid(*root) == typeid(ReturnNode))
		{
			return visit_ReturnNode(std::dynamic_pointer_cast<ReturnNode>(root), context);
		}
		else if (typeid(*root) == typeid(BreakNode))
		{
			return visit_BreakNode(std::dynamic_pointer_cast<BreakNode>(root), context);
		}
		else if (typeid(*root) == typeid(ContinueNode))
		{
			return visit_ContinueNode(std::dynamic_pointer_cast<ContinueNode>(root), context);
		}
		else
		{
			string message = Basic::format("No visit method for %s defined", typeid(*root).name());

			return RuntimeResult().failure(make_shared<RunTimeError>(root->pos_start, root->pos_end, message, context));
		}
	}

	RuntimeResult Interpreter::visit_NumberNode(shared_ptr<NumberNode> root, Context& context)
	{
		RuntimeResult res;
		Number num(root->get_tok().get_number(), root->pos_start, root->pos_end);
		num.set_context(&context);

		return res.success(make_shared<Number>(num));
	}

	RuntimeResult Interpreter::visit_StringNode(shared_ptr<StringNode> root, Context& context)
	{
		RuntimeResult res;

		String str(root->get_tok().value);
		str.set_pos(root->pos_start, root->pos_end);
		str.set_context(&context);

		return res.success(make_shared<String>(str));
	}

	RuntimeResult Interpreter::visit_ListNode(shared_ptr<ListNode> root, Context& context)
	{
		RuntimeResult res;
		vector<shared_ptr<Data>> elements;

		for (auto elem_node : root->get_element_nodes())
		{
			shared_ptr<Data> elem = res.registry(visit(elem_node, context));
			if (res.should_return())
				return res;

			if (typeid(*elem) != typeid(Data))
				elements.push_back(elem);
		}

		List result(elements);
		result.set_context(&context);
		result.set_pos(root->pos_start, root->pos_end);

		return res.success(make_shared<List>(result));
	}

	RuntimeResult Interpreter::visit_BinOpNode(shared_ptr<BinOpNode> root, Context& context)
	{
		RuntimeResult res;

		shared_ptr<ASTNode> left_node = root->get_left();
		shared_ptr<ASTNode> right_node = root->get_right();

		shared_ptr<Data> left = res.registry(visit(left_node, context));
		if (res.should_return())
			return res;

		shared_ptr<Data> right = res.registry(visit(right_node, context));
		if (res.should_return())
			return res;

		shared_ptr<Data> result;
		try
		{
			if (root->get_op().type == TD_PLUS)
			{
				result = left->added_to(right);
			}
			else if (root->get_op().type == TD_MINUS)
			{
				result = left->subbed_by(right);
			}
			else if (root->get_op().type == TD_MUL)
			{
				result = left->multed_by(right);
			}
			else if (root->get_op().type == TD_DIV)
			{
				result = left->dived_by(right);
			}
			else if (root->get_op().type == TD_POW)
			{
				result = left->powed_by(right);
			}
			else if (root->get_op().type == TD_EE)
			{
				result = left->get_comparison_eq(right);
			}
			else if (root->get_op().type == TD_NE)
			{
				result = left->get_comparison_ne(right);
			}
			else if (root->get_op().type == TD_LT)
			{
				result = left->get_comparison_lt(right);
			}
			else if (root->get_op().type == TD_GT)
			{
				result = left->get_comparison_gt(right);
			}
			else if (root->get_op().type == TD_LTE)
			{
				result = left->get_comparison_lte(right);
			}
			else if (root->get_op().type == TD_GTE)
			{
				result = left->get_comparison_gte(right);
			}
			else if (root->get_op().matches(TD_KEYWORD, "AND"))
			{
				result = left->anded_by(right);
			}
			else if (root->get_op().matches(TD_KEYWORD, "OR"))
			{
				result = left->ored_by(right);
			}

			result->set_pos(root->pos_start, root->pos_end);
			return res.success(result);
		}
		catch (RunTimeError& e)
		{
			return res.failure(make_shared<RunTimeError>(e));
		}
	}

	RuntimeResult Interpreter::visit_UnaryOpNode(shared_ptr<UnaryOpNode> root, Context& context)
	{
		RuntimeResult res;
		shared_ptr<Data> num = res.registry(visit(root->get_node(), context));
		if (res.should_return())
			return res;

		if (root->get_op().type == TD_MINUS)
			num = num->multed_by(make_shared<Number>(-1));
		else if (root->get_op().matches(TD_KEYWORD, "NOT"))
			num = num->notted();

		num->set_pos(root->pos_start, root->pos_end);

		return res.success(num);
	}

	RuntimeResult Interpreter::visit_VarAccessNode(shared_ptr<VarAccessNode> root, Context& context)
	{
		RuntimeResult res;
		string var_name = root->get_var_name_tok().value;
		shared_ptr<Data> value = context.get_symbol_table().get(var_name);

		if (value == nullptr)
			return res.failure(make_shared<RunTimeError>(root->pos_start, root->pos_end, var_name + " is not defined", context));

		// 多数情况，我们不想修改指针指向的数据，只想要一份值的拷贝
		// 但是有些时候又需要
		// 我们规定只有List可以被直接修改，其余的类型不可以
		if (typeid(*value) == typeid(Number))
		{
			shared_ptr<Number> copy_value = make_shared<Number>(*(std::dynamic_pointer_cast<Number>(value)));
			copy_value->set_context(&context);
			copy_value->set_pos(root->pos_start, root->pos_end);

			return res.success(copy_value);
		}
		else if (typeid(*value) == typeid(String))
		{
			shared_ptr<String> copy_value = make_shared<String>(*(std::dynamic_pointer_cast<String>(value)));
			copy_value->set_context(&context);
			copy_value->set_pos(root->pos_start, root->pos_end);

			return res.success(copy_value);
		}
		else if (typeid(*value) == typeid(Function))
		{
			shared_ptr<Function> copy_value = make_shared<Function>(*(std::dynamic_pointer_cast<Function>(value)));
			copy_value->set_context(&context);
			copy_value->set_pos(root->pos_start, root->pos_end);

			return res.success(copy_value);
		}
		else if (typeid(*value) == typeid(BuiltInFunction))
		{
			shared_ptr<BuiltInFunction> copy_value = make_shared<BuiltInFunction>(*(std::dynamic_pointer_cast<BuiltInFunction>(value)));
			copy_value->set_context(&context);
			copy_value->set_pos(root->pos_start, root->pos_end);

			return res.success(copy_value);
		}

		value->set_pos(root->pos_start, root->pos_end);
		value->set_context(&context);

		return res.success(value);
	}

	RuntimeResult Interpreter::visit_VarAssignNode(shared_ptr<VarAssignNode> root, Context& context)
	{
		RuntimeResult res;
		string var_name = root->get_var_name_tok().value;

		shared_ptr<Data> value = res.registry(visit(root->get_value_node(), context));
		if (res.should_return())
			return res;

		context.get_symbol_table().set(var_name, value);

		return res.success(value);
	}

	RuntimeResult Interpreter::visit_MutateNode(shared_ptr<MutateNode> root, Context& context)
	{
		RuntimeResult res;
		
		shared_ptr<ASTNode> mutant_node = root->get_mutant();
		shared_ptr<ASTNode> value_node = root->get_value();

		if (typeid(*mutant_node) == typeid(VarAccessNode))
		{
			shared_ptr<VarAccessNode> mutant = std::dynamic_pointer_cast<VarAccessNode>(mutant_node);
			string var_name = mutant->get_var_name_tok().value;

			shared_ptr<Data> value = res.registry(visit(value_node, context));
			if (res.should_return())
				return res;

			context.get_symbol_table().set(var_name, value);
			return res.success(value);
		}
		else if (typeid(*mutant_node) == typeid(IndexNode))
		{
			shared_ptr<Data> mutant = res.registry(visit(mutant_node, context));
			if (res.should_return())
				return res;

			shared_ptr<Data> value = res.registry(visit(value_node, context));
			if (res.should_return())
				return res;

			// TODO 现在只能让同类型的值成功改变，而且需要cast
			if (typeid(*mutant) != typeid(*value))
				return res.failure(make_shared<RunTimeError>(root->pos_start, root->pos_end, "Can only reassign same type of value", context));

			if (typeid(*mutant) == typeid(Number))
			{
				auto cast_mutant = std::dynamic_pointer_cast<Number>(mutant);
				auto cast_value = std::dynamic_pointer_cast<Number>(value);
				*cast_mutant = *cast_value;
			}
			else if (typeid(*mutant) == typeid(String))
			{
				auto cast_mutant = std::dynamic_pointer_cast<String>(mutant);
				auto cast_value = std::dynamic_pointer_cast<String>(value);
				*cast_mutant = *cast_value;
			}
			else if (typeid(*mutant) == typeid(List))
			{
				auto cast_mutant = std::dynamic_pointer_cast<List>(mutant);
				auto cast_value = std::dynamic_pointer_cast<List>(value);
				*cast_mutant = *cast_value;
			}

			return res.success(value);
		}

		return res.failure(make_shared<RunTimeError>(root->pos_start, root->pos_end, "Cannot assign value to literal", context));
	}

	RuntimeResult Interpreter::visit_IndexNode(shared_ptr<IndexNode> root, Context& context)
	{
		RuntimeResult res;
		shared_ptr<Data> value = res.registry(visit(root->get_value(), context));
		if (res.should_return())
			return res;

		if (typeid(*value) == typeid(List) || typeid(*value) == typeid(String))
		{
			try
			{
				shared_ptr<Data> index = res.registry(visit(root->get_index(), context));
				shared_ptr<Data> result = value->index_by(index);
				result->set_pos(root->pos_start, root->pos_end);
				result->set_context(&context);

				return res.success(result);
			}
			catch (RunTimeError& e)
			{
				return res.failure(make_shared<RunTimeError>(e));
			}
		}
		else
			return res.failure(make_shared<RunTimeError>(root->pos_start, root->pos_end, "Can only index List and String", context));
	}

	RuntimeResult Interpreter::visit_IfNode(shared_ptr<IfNode> root, Context& context)
	{
		RuntimeResult res;

		for (auto elem : root->get_cases())
		{
			shared_ptr<ASTNode> condition_node = std::get<0>(elem);
			shared_ptr<ASTNode> expr_node = std::get<1>(elem);
			bool should_return_null = std::get<2>(elem);

			shared_ptr<Number> condition_value = std::dynamic_pointer_cast<Number>(res.registry(visit(condition_node, context)));
			if (res.should_return())
				return res;

			if (condition_value->is_true())
			{
				shared_ptr<Data> expr_value = res.registry(visit(expr_node, context));
				if (res.should_return())
					return res;

				if (should_return_null)
					return res.success(make_shared<Data>(Number::null));
				else
					return res.success(expr_value);
			}
		}

		Else_Case else_case = root->get_else_case();
		shared_ptr<ASTNode> else_node = std::get<0>(else_case);
		if (else_node != nullptr)
		{
			shared_ptr<Data> else_value = res.registry(visit(else_node, context));
			if (res.should_return())
				return res;

			if (std::get<1>(else_case))
				return res.success(make_shared<Data>(Number::null));
			else
				return res.success(else_value);
		}

		return res.success(make_shared<Data>(Number::null));
	}

	RuntimeResult Interpreter::visit_ForNode(shared_ptr<ForNode> root, Context& context)
	{
		RuntimeResult res;
		vector<shared_ptr<Data>> elements;

		shared_ptr<Number> start_data = std::dynamic_pointer_cast<Number>(res.registry(visit(root->get_start_value_node(), context)));
		if (res.should_return())
			return res;

		shared_ptr<Number> end_data = std::dynamic_pointer_cast<Number>(res.registry(visit(root->get_end_value_node(), context)));
		if (res.should_return())
			return res;

		shared_ptr<ASTNode> step_node = root->get_step_value_node();
		shared_ptr<Number> step_data = make_shared<Number>(1);
		if (step_node != nullptr)
		{
			step_data = std::dynamic_pointer_cast<Number>(res.registry(visit(step_node, context)));
			if (res.should_return())
				return res;
		}

		int i = start_data->get_value(true);
		int end_value = end_data->get_value(true);
		int step_value = step_data->get_value(true);

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
			context.get_symbol_table().set(root->get_var_name_tok().value, make_shared<Number>(i));
			i += step_value;

			shared_ptr<Data> elem = res.registry(visit(root->get_body_node(), context));
			if (res.should_return() && !res.should_break() && !res.should_continue())
				return res;

			if (res.should_continue())
				continue;

			if (res.should_break())
				break;

			if (typeid(*elem) != typeid(Data))
				elements.push_back(elem);
		}

		if (root->is_return_null())
			return res.success(make_shared<Data>(Number::null));
		else
		{
			if (!elements.empty())
			{
				List result(elements);
				result.set_context(&context);
				result.set_pos(root->pos_start, root->pos_end);
				return res.success(make_shared<List>(result));
			}

			return res.success(make_shared<Data>(Number::null));
		}
	}

	RuntimeResult Interpreter::visit_WhileNode(shared_ptr<WhileNode> root, Context& context)
	{
		RuntimeResult res;
		vector<shared_ptr<Data>> elements;

		while (true)
		{
			shared_ptr<Number> condition = std::dynamic_pointer_cast<Number>(res.registry(visit(root->get_condition_node(), context)));
			if (res.should_return())
				return res;

			if (!condition->is_true())
				break;

			shared_ptr<Data> elem = res.registry(visit(root->get_body_node(), context));
			if (res.should_return() && !res.should_break() && !res.should_continue())
				return res;

			if (res.should_continue())
				continue;

			if (res.should_break())
				break;

			if (typeid(*elem) != typeid(Data))
				elements.push_back(elem);
		}

		if (root->is_return_null())
			return res.success(make_shared<Data>(Number::null));
		else
		{
			if (!elements.empty())
			{
				List result(elements);
				result.set_context(&context);
				result.set_pos(root->pos_start, root->pos_end);
				return res.success(make_shared<List>(result));
			}

			return res.success(make_shared<Data>(Number::null));
		}
	}

	RuntimeResult Interpreter::visit_FuncDefNode(shared_ptr<FuncDefNode> root, Context& context)
	{
		RuntimeResult res;

		string func_name = root->get_var_name_tok().value;
		shared_ptr<ASTNode> body_node = root->get_body_node();

		vector<string> arg_names;
		for (Token tok : root->get_arg_name_toks())
		{
			arg_names.push_back(tok.value);
		}

		shared_ptr<Function> func = make_shared<Function>(func_name, body_node, arg_names, root->is_auto_return());
		func->set_pos(root->pos_start, root->pos_end);

		if (!root->isAnonymous())
		{
			context.get_symbol_table().set(func_name, func);
		}
		func->set_context(&context);

		return res.success(func);
	}

	RuntimeResult Interpreter::visit_CallNode(shared_ptr<CallNode> root, Context& context)
	{
		RuntimeResult res;
		vector<shared_ptr<Data>> args;

		shared_ptr<Data> value_to_call = res.registry(visit(root->get_func_node(), context));
		if (res.should_return())
			return res;

		value_to_call->set_pos(root->pos_start, root->pos_end);

		for (auto arg_node : root->get_args_nodes())
		{
			args.push_back(res.registry(visit(arg_node, context)));
			if (res.should_return())
				return res;
		}

		shared_ptr<Data> return_value = res.registry(value_to_call->execute(args));
		if (res.should_return())
			return res;

		return_value->set_pos(root->pos_start, root->pos_end);
		return_value->set_context(&context);

		return res.success(return_value);
	}

	RuntimeResult Interpreter::visit_ReturnNode(shared_ptr<ReturnNode> root, Context& context)
	{
		RuntimeResult res;

		shared_ptr<ASTNode> return_node = root->get_return_node();
		shared_ptr<Data> return_value;
		if (return_node != nullptr)
		{
			return_value = res.registry(visit(return_node, context));
			if (res.should_return())
				return res;
		}
		else
			return_value = make_shared<Data>(Number::null);

		return res.success_return(return_value);
	}

	RuntimeResult Interpreter::visit_BreakNode(shared_ptr<BreakNode> root, Context& context)
	{
		return RuntimeResult().success_break();
	}

	RuntimeResult Interpreter::visit_ContinueNode(shared_ptr<ContinueNode> root, Context& context)
	{
		return RuntimeResult().success_continue();
	}
}