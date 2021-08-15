#include "../../include/Common/utils.h"
#include "../../include/Interpreter/Data.h"
#include "../../include/Interpreter/Interpreter.h"
#include "../../include/Interpreter/RunTimeError.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <fstream>

namespace Basic {

	void Data::set_pos(Position& start, Position& end)
	{
		this->pos_start = start;
		this->pos_end = end;
	}

	void Data::set_context(Context* context)
	{
		this->context = context;
	}

	Number::Number(double value, Position start, Position end, Context* context)
	{
		this->value = value;
		set_pos(start, end);
		set_context(context);
	}

	Number::Number(const Number& other)
	{
		this->pos_start = other.pos_start;
		this->pos_end = other.pos_end;
		this->value = other.value;
		this->context = other.context;
	}

	double Number::get_value(bool wantInt)
	{
		if (wantInt)
			return (int)this->value;

		return this->value;
	}

	shared_ptr<Data> Number::added_to(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);
			Number result(this->value + other_num->value);
			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::subbed_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);
			Number result(this->value - other_num->value);
			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::multed_by(shared_ptr<Data> other)
	{
		if (typeid(*other) == typeid(Number))
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);
			Number result(this->value * other_num->value);
			result.set_context(this->context);
			return make_shared<Number>(result);
		}
		else if (typeid(*other) == typeid(String))
		{
			shared_ptr<String> other_str = std::dynamic_pointer_cast<String>(other);

			return other_str->multed_by(make_shared<Number>(*this));
		}
		else
		{
			illegal_operation(other);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::dived_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);
			if (other_num->value == 0)
				throw RunTimeError(other->pos_start, other->pos_end, "Division by 0", *this->context);

			Number result(this->value / other_num->value);
			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::powed_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			Number result(pow(this->value, other_num->value));
			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::get_comparison_eq(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			Number result(0);
			if (this->value == other_num->value)
				result.value = 1;

			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::get_comparison_ne(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			Number result(0);
			if (this->value != other_num->value)
				result.value = 1;

			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::get_comparison_lt(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			Number result(0);
			if (this->value < other_num->value)
				result.value = 1;

			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::get_comparison_gt(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			Number result(0);
			if (this->value > other_num->value)
				result.value = 1;

			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::get_comparison_lte(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			Number result(0);
			if (this->value <= other_num->value)
				result.value = 1;

			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::get_comparison_gte(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			Number result(0);
			if (this->value >= other_num->value)
				result.value = 1;

			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::anded_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			Number result((int)this->value & (int)other_num->value);
			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::ored_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			Number result((int)this->value | (int)other_num->value);
			result.set_context(this->context);
			return make_shared<Number>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> Number::notted()
	{
		Number result(0);
		if (this->value == 0)
			result.value = 1;
		result.set_context(this->context);
		return make_shared<Number>(result);
	}

	bool Number::is_true()
	{
		return this->value != 0;
	}

	string Number::__repr__()
	{
		if ((int)this->value == this->value)
			return std::to_string((int)this->value);
		return std::to_string(this->value);
	}

	String::String(string value)
	{
		this->value = value;
	}

	String::String(const String& other)
	{
		this->value = other.value;
		this->pos_start = other.pos_start;
		this->pos_end = other.pos_end;
		this->context = other.context;
	}

	shared_ptr<Data> String::added_to(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(String))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<String> other_str = std::dynamic_pointer_cast<String>(other);

			String result(this->value + other_str->value);
			result.set_context(this->context);

			return make_shared<String>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> String::multed_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);

			string str = this->value;
			for (size_t i = 1; i < other_num->get_value(true); i++)
				str += this->value;

			String result(str);
			result.set_context(this->context);

			return make_shared<String>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> String::index_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);
			int index = other_num->get_value(true);

			// 越界会触发exception
			try
			{
				if (index >= 0)
					return make_shared<String>(string(1,value.at(index)));
				else
					return make_shared<String>(string(1, value.at(value.length() + index)));
			}
			catch (const std::exception& e)
			{
				throw RunTimeError(other->pos_start, other->pos_end, "String fetch, element out of bound", *this->context);
			}
		}

		return nullptr;
	}

	bool String::is_true()
	{
		return this->value.length() > 0;
	}

	string String::__repr__()
	{
		return Basic::format("\"%s\"",value.c_str());
	}

	string String::__str__()
	{
		return value;
	}

	string String::getValue()
	{
		return this->value;
	}

	List::List(vector<shared_ptr<Data>>& elems)
	{
		this->elements = elems;
	}

	List::List(const List& other)
	{
		this->elements = other.elements;
		this->pos_start = other.pos_start;
		this->pos_end = other.pos_end;
		this->context = other.context;
	}

	shared_ptr<Data> List::added_to(shared_ptr<Data> other)
	{
		List result(*this);
		result.elements.push_back(other);
		return make_shared<List>(result);
	}

	shared_ptr<Data> List::multed_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(List))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<List> other_list = std::dynamic_pointer_cast<List>(other);
			List result(*this);
			for (auto elem : other_list->elements)
				result.elements.push_back(elem);

			return make_shared<List>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> List::subbed_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);
			List result(*this);

			int index = other_num->get_value(true);

			if (index >= 0 && index < result.elements.size())
			{
				result.elements.erase(result.elements.begin() + index);
			}
			else if (index < 0 && (-index) < result.elements.size())
			{
				result.elements.erase(result.elements.end() + index);
			}
			else
				throw RunTimeError(other->pos_start, other->pos_end, "Element can't be removed, Index out of bound", *this->context);

			return make_shared<List>(result);
		}

		return nullptr;
	}

	shared_ptr<Data> List::index_by(shared_ptr<Data> other)
	{
		if (typeid(*other) != typeid(Number))
		{
			illegal_operation(other);
		}
		else
		{
			shared_ptr<Number> other_num = std::dynamic_pointer_cast<Number>(other);
			int index = other_num->get_value(true);

			// 越界会触发exception
			try
			{
				if (index >= 0)
					return elements.at(index);
				else
					return elements.at(elements.size() + index);
			}
			catch (const std::exception& e)
			{
				throw RunTimeError(other->pos_start, other->pos_end, "List fetch, element out of bound", *this->context);
			}
		}

		return nullptr;
	}

	string List::__repr__()
	{
		string result = "[";

		for (auto elem : elements)
		{
			result += elem->__repr__();
			result.push_back(',');
		}

		if (result != "[")
			result.pop_back();

		result.push_back(']');

		return result;
	}

	vector<shared_ptr<Data>>& List::get_elements()
	{
		return this->elements;
	}

	BaseFunction::BaseFunction(string& func_name)
	{
		this->func_name = func_name;
	}

	BaseFunction::BaseFunction(const BaseFunction& other)
	{
		this->func_name = other.func_name;
		this->pos_start = other.pos_start;
		this->pos_end = other.pos_end;
		this->context = other.context;
	}

	Context BaseFunction::generate_new_context()
	{
		Context func_context(this->func_name, make_shared<Context>(*this->context), this->pos_start);

		func_context.set_symbol_table(SymbolTable(make_shared<SymbolTable>((func_context.get_parent()->get_symbol_table()))));

		return func_context;
	}

	RuntimeResult BaseFunction::check_args(const vector<string>& arg_names, const vector<shared_ptr<Data>>& args)
	{
		RuntimeResult res;
		if (args.size() > arg_names.size())
		{
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, std::to_string(args.size() - arg_names.size()) + " too many args passed into " + this->func_name, *this->context));
		}

		if (args.size() < arg_names.size())
		{
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, std::to_string(arg_names.size() - args.size()) + " too few args passed into " + this->func_name, *this->context));
		}

		return res.success(nullptr);
	}

	void BaseFunction::populate_args(const vector<string>& arg_names, vector<shared_ptr<Data>>& args, Context& exec_ctx)
	{
		for (size_t i = 0; i < args.size(); i++)
		{
			string arg_name = arg_names[i];
			shared_ptr<Data> arg_value = args[i];
			exec_ctx.get_symbol_table().set(arg_name, arg_value);
		}

		for (auto arg_value : args)
		{
			arg_value->set_context(&exec_ctx);
		}
	}

	RuntimeResult BaseFunction::check_populate_args(const vector<string>& arg_names, vector<shared_ptr<Data>>& args, Context& exec_ctx)
	{
		RuntimeResult res;
		res.registry(check_args(arg_names, args));
		if (res.should_return())
			return res;

		populate_args(arg_names, args, exec_ctx);

		return res.success(nullptr);
	}

	string BaseFunction::__repr__()
	{
		return Basic::format("<function %s>", func_name.c_str());
	}

	Function::Function(string& func_name, shared_ptr<ASTNode> body_node, vector<string>& arg_names, bool auto_return) : BaseFunction(func_name)
	{
		this->body_node = body_node;
		this->arg_names = arg_names;
		this->auto_return = auto_return;
	}

	Function::Function(const Function& other) : BaseFunction(other)
	{
		this->body_node = other.body_node;
		this->arg_names = other.arg_names;
		this->auto_return = other.auto_return;
	}

	RuntimeResult Function::execute(vector<shared_ptr<Data>>& args)
	{
		RuntimeResult res;
		Interpreter interpreter;
		Context func_context = generate_new_context();

		res.registry(check_populate_args(this->arg_names, args, func_context));
		if (res.should_return())
			return res;

		shared_ptr<Data> value = res.registry(interpreter.visit(body_node, func_context));
		if (res.should_return())
			return res;

		shared_ptr<Data> func_return_value = res.get_func_return_value();
		if (res.should_return() && func_return_value == nullptr)
			return res;

		// 正常情况。单行返回value，多行有return返回return的值，没有返回空
		shared_ptr<Data> return_value = this->auto_return ? value : nullptr;

		// in case有人在单行中写return语句，所以我们这样写，可以确保单行也可以使用return的值
		if (return_value == nullptr && func_return_value != nullptr)
			return_value = func_return_value;
		else if (return_value == nullptr && func_return_value == nullptr)
			return_value = make_shared<Data>(Number::null);

		return res.success(return_value);
	}

	BuiltInFunction::BuiltInFunction(string func_name) : BaseFunction(func_name)
	{
	}

	BuiltInFunction::BuiltInFunction(const BuiltInFunction& other) : BaseFunction(other)
	{
	}

	string BuiltInFunction::__repr__()
	{
		string result = Basic::format("<built-in function %s>(",func_name.c_str());
		for (string arg_name : func_args_map.find(func_name)->second)
		{
			result += arg_name;
			result += ",";
		}
		result.pop_back();
		result += ")";

		return result;
	}

	RuntimeResult BuiltInFunction::execute(vector<shared_ptr<Data>>& args)
	{
		RuntimeResult res;
		Context func_context = generate_new_context();

		auto method_ptr = func_name_map.find(func_name);

		function<RuntimeResult(BuiltInFunction*, Context&)> method;
		if (method_ptr != func_name_map.end())
			method = method_ptr->second;
		else
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "No execution method_" + this->func_name + " defined", func_context));

		// 找到函数了，那么必然有对应的参数列表
		// 所以不检查find的合法性了
		res.registry(check_populate_args(func_args_map.find(func_name)->second, args, func_context));
		if (res.should_return())
			return res;

		shared_ptr<Data> return_value = res.registry(method(this, func_context));
		if (res.should_return())
			return res;

		return res.success(return_value);
	}

	RuntimeResult BuiltInFunction::execute_run(Context& exec_ctx)
	{
		RuntimeResult res;

		shared_ptr<Data> filename_node = exec_ctx.get_symbol_table().get("filename");
		if (typeid(*filename_node) != typeid(String))
		{
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Filename must be String", exec_ctx));
		}

		string filename = std::dynamic_pointer_cast<String>(filename_node)->getValue();

		std::ifstream ifs;
		ifs.exceptions(std::ifstream::failbit);
		try
		{
			ifs.open(filename);
			string text((std::istreambuf_iterator<char>(ifs)),
				(std::istreambuf_iterator<char>()));
			auto result = Basic::run(filename, text);

			if (std::get<1>(result) != nullptr)
			{
				return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Failed to finish executing script " + filename + "\n\n" + std::get<1>(result)->as_string(), exec_ctx));
			}
			ifs.close();
		}
		catch (const std::exception& e)
		{
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Failed to load script " + filename + "\n" + e.what(), exec_ctx));
		}

		return make_shared<Data>(Number::null);
	}

	RuntimeResult BuiltInFunction::execute_print(Context& exec_ctx)
	{
		RuntimeResult res;
		shared_ptr<Data> value = exec_ctx.get_symbol_table().get("value");

		if (typeid(*value) == typeid(String))
			Basic::printf("%s\n", std::dynamic_pointer_cast<String>(value)->__str__());
		else
			Basic::printf("%s\n", value->__repr__());

		return res.success(make_shared<Data>(Number::null));
	}

	RuntimeResult BuiltInFunction::execute_prints(Context& exec_ctx)
	{
		RuntimeResult res;

		shared_ptr<Data> values = exec_ctx.get_symbol_table().get("values");
		shared_ptr<Data> ends_with = exec_ctx.get_symbol_table().get("ends_with");

		if (typeid(*values) != typeid(List))
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Expected a List of elements for first argument", exec_ctx));
		if (typeid(*ends_with) != typeid(String))
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Expected a String to end_with for second argument", exec_ctx));

		shared_ptr<List> list_node = std::dynamic_pointer_cast<List>(values);
		shared_ptr<String> end_node = std::dynamic_pointer_cast<String>(ends_with);
		vector<shared_ptr<Data>> list = list_node->get_elements();
		string end_value = end_node->getValue();

		for (shared_ptr<Data>& elem : list)
		{
			if (typeid(*elem) == typeid(String))
				Basic::printf("%s ", std::dynamic_pointer_cast<String>(elem)->__str__());
			else
				Basic::printf("%s ", elem->__repr__());
		}
		std::cout << end_value;

		return res.success(make_shared<Data>(Number::null));
	}

	RuntimeResult BuiltInFunction::execute_print_ret(Context& exec_ctx)
	{
		RuntimeResult res;
		shared_ptr<Data> value = exec_ctx.get_symbol_table().get("value");
		string repr;

		if (typeid(*value) == typeid(String))
			repr = std::dynamic_pointer_cast<String>(value)->__str__();
		else
			repr = value->__repr__();

		return res.success(make_shared<String>(repr));
	}

	RuntimeResult BuiltInFunction::execute_input(Context& exec_ctx)
	{
		RuntimeResult res;
		string text;
		getline(std::cin, text);
		return res.success(make_shared<String>(text));
	}

	RuntimeResult BuiltInFunction::execute_input_num(Context& exec_ctx)
	{
		RuntimeResult res;
		string input_value;
		while (true)
		{
			getline(std::cin, input_value);
			if (Basic::isNumber(input_value))
			{
				return input_value.find('.') == string::npos ? res.success(make_shared<Number>(atoi(input_value.c_str()))) : res.success(make_shared<Number>(atof(input_value.c_str())));
			}
			else
			{
				std::cout << "Input must be a Number, Try again!\n";
			}
		}

		return res.success(make_shared<Data>(Number::null));
	}

	RuntimeResult BuiltInFunction::execute_clear(Context& exec_ctx)
	{
		RuntimeResult res;
#if defined _WIN32
		system("cls");
		//clrscr(); // including header file : conio.h
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
		system("clear");
#elif defined(__APPLE__)
		system("clear");
#endif
		return res.success(make_shared<Data>(Number::null));
	}

	RuntimeResult BuiltInFunction::execute_is_number(Context& exec_ctx)
	{
		RuntimeResult res;
		shared_ptr<Data> value = exec_ctx.get_symbol_table().get("value");
		if (typeid(*value) == typeid(Number))
		{
			return res.success(make_shared<Number>(Number::TRUE));
		}

		return res.success(make_shared<Number>(Number::FALSE));
	}

	RuntimeResult BuiltInFunction::execute_is_string(Context& exec_ctx)
	{
		RuntimeResult res;
		shared_ptr<Data> value = exec_ctx.get_symbol_table().get("value");
		if (typeid(*value) == typeid(String))
		{
			return res.success(make_shared<Number>(Number::TRUE));
		}

		return res.success(make_shared<Number>(Number::FALSE));
	}

	RuntimeResult BuiltInFunction::execute_is_list(Context& exec_ctx)
	{
		RuntimeResult res;
		shared_ptr<Data> value = exec_ctx.get_symbol_table().get("value");
		if (typeid(*value) == typeid(List))
		{
			return res.success(make_shared<Number>(Number::TRUE));
		}

		return res.success(make_shared<Number>(Number::FALSE));
	}

	RuntimeResult BuiltInFunction::execute_is_function(Context& exec_ctx)
	{
		RuntimeResult res;
		shared_ptr<Data> value = exec_ctx.get_symbol_table().get("value");
		if (typeid(*value) == typeid(Function) || typeid(*value) == typeid(BuiltInFunction))
		{
			return res.success(make_shared<Number>(Number::TRUE));
		}

		return res.success(make_shared<Number>(Number::FALSE));
	}

	RuntimeResult BuiltInFunction::execute_len(Context& exec_ctx)
	{
		RuntimeResult res;
		shared_ptr<Data> value_node = exec_ctx.get_symbol_table().get("value");

		if (typeid(*value_node) == typeid(List))
		{
			shared_ptr<List> list_node = std::dynamic_pointer_cast<List>(value_node);
			return res.success(make_shared<Number>(list_node->get_elements().size()));
		}
		else if (typeid(*value_node) == typeid(String))
		{
			shared_ptr<String> list_node = std::dynamic_pointer_cast<String>(value_node);
			return res.success(make_shared<Number>(list_node->getValue().length()));
		}
		else
		{
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Argument can only be String/List", *this->context));
		}

		return res.success(make_shared<Number>(Number::null));
	}

	RuntimeResult BuiltInFunction::execute_append(Context& exec_ctx)
	{
		RuntimeResult res;

		shared_ptr<Data> first_arg = exec_ctx.get_symbol_table().get("list");
		shared_ptr<Data> second_arg = exec_ctx.get_symbol_table().get("value");

		if (typeid(*first_arg) != typeid(List))
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "First argument must be a list", *this->context));

		shared_ptr<List> list = std::dynamic_pointer_cast<List>(first_arg);

		// 这将直接插入到原本的第一参数中
		// 故该函数为mutable，而added_to会返回一个新的值
		list->get_elements().push_back(second_arg);

		return res.success(make_shared<Data>(Number::TRUE));
	}

	RuntimeResult BuiltInFunction::execute_pop(Context& exec_ctx)
	{
		RuntimeResult res;

		shared_ptr<Data> first_arg = exec_ctx.get_symbol_table().get("list");
		shared_ptr<Data> second_arg = exec_ctx.get_symbol_table().get("index");

		if (typeid(*first_arg) != typeid(List))
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "First argument must be a list", *this->context));

		if (typeid(*second_arg) != typeid(Number))
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Second argument must be an index(Number)", *this->context));

		shared_ptr<List> list = std::dynamic_pointer_cast<List>(first_arg);
		shared_ptr<Number> index = std::dynamic_pointer_cast<Number>(second_arg);
		shared_ptr<Data> return_data = make_shared<Data>(Number::FALSE);

		vector<shared_ptr<Data>>& list_value = list->get_elements();
		int index_value = index->get_value(true);

		if (index_value >= 0 && index_value < list_value.size())
		{
			return_data = list_value[index_value];
			list_value.erase(list_value.begin() + index_value);
		}
		else if (index_value < 0 && (-index_value) < list_value.size())
		{
			return_data = list_value[list_value.size() + index_value];
			list_value.erase(list_value.end() + index_value);
		}
		else
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Element can't be poped, Index out of bound", *this->context));

		return res.success(return_data);
	}

	RuntimeResult BuiltInFunction::execute_pop_front(Context& exec_ctx)
	{
		RuntimeResult res;

		shared_ptr<Data> list_ptr = exec_ctx.get_symbol_table().get("list");

		if (typeid(*list_ptr) != typeid(List))
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "First argument must be a list", *this->context));

		shared_ptr<List> list = std::dynamic_pointer_cast<List>(list_ptr);
		shared_ptr<Data> return_data = make_shared<Data>(Number::null);

		vector<shared_ptr<Data>>& list_value = list->get_elements();

		if (list_value.size() > 0)
		{
			return_data = list_value[0];
			list_value.erase(list_value.begin());
		}

		return res.success(return_data);
	}

	RuntimeResult BuiltInFunction::execute_pop_back(Context& exec_ctx)
	{
		RuntimeResult res;
		shared_ptr<Data> list_ptr = exec_ctx.get_symbol_table().get("list");

		if (typeid(*list_ptr) != typeid(List))
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "First argument must be a list", *this->context));

		shared_ptr<List> list = std::dynamic_pointer_cast<List>(list_ptr);
		shared_ptr<Data> return_data = make_shared<Data>(Number::null);

		vector<shared_ptr<Data>>& list_value = list->get_elements();

		if (list_value.size() > 0)
		{
			return_data = (*list_value.rbegin());
			list_value.pop_back();
		}

		return res.success(return_data);
	}

	RuntimeResult BuiltInFunction::execute_extend(Context& exec_ctx)
	{
		RuntimeResult res;
		shared_ptr<Data> first_arg = exec_ctx.get_symbol_table().get("list1");
		shared_ptr<Data> second_arg = exec_ctx.get_symbol_table().get("list2");

		if (typeid(*first_arg) != typeid(List))
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Both arguments must be list", *this->context));

		if (typeid(*second_arg) != typeid(List))
			return res.failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Both arguments must be list", *this->context));

		shared_ptr<List> list1 = std::dynamic_pointer_cast<List>(first_arg);
		shared_ptr<List> list2 = std::dynamic_pointer_cast<List>(second_arg);

		vector<shared_ptr<Data>>& list1_value = list1->get_elements();
		vector<shared_ptr<Data>>& list2_value = list2->get_elements();

		for (auto data : list2_value)
			list1_value.push_back(data);

		return res.success(make_shared<Data>(Number::TRUE));
	}

	// 静态成员赋值

	const Number Number::null = Number(0);
	const Number Number::FALSE = Number(0);
	const Number Number::TRUE = Number(1);
	const Number Number::MATH_PI = Number(3.14159265354);

	const map<string, function<RuntimeResult(BuiltInFunction *, Context &)>> BuiltInFunction::func_name_map = map<string, function<RuntimeResult(BuiltInFunction *, Context &)>>{
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("RUN", &BuiltInFunction::execute_run),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("PRINT", &BuiltInFunction::execute_print),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("PRINTS", &BuiltInFunction::execute_prints),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("PRINT_RET", &BuiltInFunction::execute_print_ret),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("INPUT", &BuiltInFunction::execute_input),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("INPUT_NUM", &BuiltInFunction::execute_input_num),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("CLEAR", &BuiltInFunction::execute_clear),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("IS_NUM", &BuiltInFunction::execute_is_number),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("IS_STR", &BuiltInFunction::execute_is_string),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("IS_LIST", &BuiltInFunction::execute_is_list),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("IS_FUNC", &BuiltInFunction::execute_is_function),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("LEN", &BuiltInFunction::execute_len),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("APPEND", &BuiltInFunction::execute_append),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("POP", &BuiltInFunction::execute_pop),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("POP_BACK", &BuiltInFunction::execute_pop_back),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("POP_FRONT", &BuiltInFunction::execute_pop_front),
		pair<string, function<RuntimeResult(BuiltInFunction *, Context &)>>("EXTEND", &BuiltInFunction::execute_extend)};

	const map<string, vector<string>> BuiltInFunction::func_args_map = map<string, vector<string>>{
		pair<string, vector<string>>("RUN", vector<string>{"filename"}),
		pair<string, vector<string>>("PRINT", vector<string>{"value"}),
		pair<string, vector<string>>("PRINTS", vector<string>{"values", "ends_with"}),
		pair<string, vector<string>>("PRINT_RET", vector<string>{"value"}),
		pair<string, vector<string>>("INPUT", vector<string>()),
		pair<string, vector<string>>("INPUT_NUM", vector<string>()),
		pair<string, vector<string>>("CLEAR", vector<string>()),
		pair<string, vector<string>>("IS_NUM", vector<string>{"value"}),
		pair<string, vector<string>>("IS_STR", vector<string>{"value"}),
		pair<string, vector<string>>("IS_LIST", vector<string>{"value"}),
		pair<string, vector<string>>("IS_FUNC", vector<string>{"value"}),
		pair<string, vector<string>>("LEN", vector<string>{"value"}),
		pair<string, vector<string>>("APPEND", vector<string>{"list", "value"}),
		pair<string, vector<string>>("POP", vector<string>{"list", "index"}),
		pair<string, vector<string>>("POP_BACK", vector<string>{"list"}),
		pair<string, vector<string>>("POP_FRONT", vector<string>{"list"}),
		pair<string, vector<string>>("EXTEND", vector<string>{"list1", "list2"})};

}