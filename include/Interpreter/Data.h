#pragma once

#include <memory>
#include <functional>
#include <map>
#include "../Common/Position.h"
#include "../Common/Context.h"
#include "../Parser/Node.h"
#include "RuntimeResult.h"
#include "RunTimeError.h"

using std::function;
using std::map;
using std::pair;
using std::shared_ptr;
using std::weak_ptr;

namespace Basic {

	class Data
	{
	public:
		void set_pos(Position& start, Position& end);
		void set_context(Context*);

		// 算数运算
		virtual shared_ptr<Data> added_to(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> subbed_by(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> multed_by(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> dived_by(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> powed_by(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}

		// 布尔运算
		virtual shared_ptr<Data> get_comparison_eq(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> get_comparison_ne(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> get_comparison_lt(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> get_comparison_gt(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> get_comparison_lte(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> get_comparison_gte(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}

		// 逻辑运算
		virtual shared_ptr<Data> anded_by(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> ored_by(const shared_ptr<Data>& other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual shared_ptr<Data> notted()
		{
			illegal_operation();
			return nullptr;
		}

		// Callable
		virtual RuntimeResult execute(vector<shared_ptr<Data>>& args)
		{
			illegal_operation();
			return RuntimeResult();
		}

		// 下标索引
		virtual shared_ptr<Data> index_by(const shared_ptr<Data>& other)
		{
			illegal_operation();
			return nullptr;
		}

		virtual bool is_true() { return false; }
		virtual string __repr__() { return ""; }

		void illegal_operation(shared_ptr<Data> other = nullptr)
		{
			if (other == nullptr)
				throw RunTimeError(this->pos_start, this->pos_end, "Illegal operation", *this->context);
			else
				throw RunTimeError(this->pos_start, other->pos_end, "Illegal operation", *this->context);
		}

	public:
		Position pos_start;
		Position pos_end;
		Context* context;
	};

	class Number : public Data
	{
	public:
		Number(double value = 0, Position start = Position(), Position end = Position(), Context* = nullptr);
		Number(const Number&);
		double get_value(bool wantInt = false);

		shared_ptr<Data> added_to(const shared_ptr<Data>& other) override;
		shared_ptr<Data> subbed_by(const shared_ptr<Data>& other) override;
		shared_ptr<Data> multed_by(const shared_ptr<Data>& other) override;
		shared_ptr<Data> dived_by(const shared_ptr<Data>& other) override;
		shared_ptr<Data> powed_by(const shared_ptr<Data>& other) override;

		shared_ptr<Data> get_comparison_eq(const shared_ptr<Data>& other) override;
		shared_ptr<Data> get_comparison_ne(const shared_ptr<Data>& other) override;
		shared_ptr<Data> get_comparison_lt(const shared_ptr<Data>& other) override;
		shared_ptr<Data> get_comparison_gt(const shared_ptr<Data>& other) override;
		shared_ptr<Data> get_comparison_lte(const shared_ptr<Data>& other) override;
		shared_ptr<Data> get_comparison_gte(const shared_ptr<Data>& other) override;

		shared_ptr<Data> anded_by(const shared_ptr<Data>& other) override;
		shared_ptr<Data> ored_by(const shared_ptr<Data>& other) override;
		shared_ptr<Data> notted() override;

		// Number类同样掌管bool判断，所以有判断是否为真的功能
		bool is_true() override;

		string __repr__() override;

		static const Number null;
		static const Number TRUE;
		static const Number FALSE;
		static const Number MATH_PI;

	private:
		double value;
	};

	class String : public Data
	{
	public:
		String(string value);
		String(const String&);

		shared_ptr<Data> added_to(const shared_ptr<Data>&) override;
		shared_ptr<Data> multed_by(const shared_ptr<Data>&) override;

		shared_ptr<Data> index_by(const shared_ptr<Data>&) override;

		bool is_true() override;
		string __repr__() override;
		string __str__(); // Print时不希望带有引号
		string getValue();

	private:
		string value;
	};

	class List : public Data
	{
	public:
		List(vector<shared_ptr<Data>>& elems);
		List(const List&);

		// add new elem
		shared_ptr<Data> added_to(const shared_ptr<Data>&) override;

		// combine two list
		shared_ptr<Data> multed_by(const shared_ptr<Data>&) override;

		// remove elem of the given index(Number)
		shared_ptr<Data> subbed_by(const shared_ptr<Data>&) override;
		
		// get elem of given index(Number)
		shared_ptr<Data> index_by(const shared_ptr<Data>&) override;

		string __repr__() override;

		vector<shared_ptr<Data>>& get_elements();

	private:
		vector<shared_ptr<Data>> elements;
	};

	// 函数类的基类，封装了公有行为
	class BaseFunction : public Data
	{
	public:
		BaseFunction(string& func_name);
		BaseFunction(const BaseFunction& other);

		// 生成函数的“上下文”
		Context generate_new_context();

		// 检查参数个数是否匹配，若匹配则加入到Context中
		RuntimeResult check_populate_args(const vector<string>& arg_names, vector<shared_ptr<Data>>& args, Context& exec_ctx);

		string __repr__() override;

	protected:
		string func_name;

	private:
		// 检查参数个数是否匹配
		RuntimeResult check_args(const vector<string>& arg_names, const vector<shared_ptr<Data>>& args);

		// 将<参数-值>对，加入到Symbol_Table中
		void populate_args(const vector<string>& arg_names, vector<shared_ptr<Data>>& args, Context& exec_ctx);
	};

	class Function : public BaseFunction
	{
	public:
		Function(string& func_name, shared_ptr<ASTNode> body_node, vector<string>& arg_names, bool auto_return = true);
		Function(const Function&);

		RuntimeResult execute(vector<shared_ptr<Data>>& args) override;

	private:
		shared_ptr<ASTNode> body_node;
		vector<string> arg_names;
		bool auto_return;
	};

	class BuiltInFunction : public BaseFunction
	{
	public:
		BuiltInFunction(string func_name);
		BuiltInFunction(const BuiltInFunction&);

		string __repr__() override;

		RuntimeResult execute(vector<shared_ptr<Data>>& args) override;

		// 内置函数

		// 运行外部文件
		RuntimeResult execute_run(Context& exec_ctx);

		// 输出value
		RuntimeResult execute_print(Context& exec_ctx);

		// 将传入的列表中的元素以空格拼接，输出
		RuntimeResult execute_prints(Context& exec_ctx);

		// 返回value
		RuntimeResult execute_print_ret(Context& exec_ctx);

		// 输入（以字符串存储）
		RuntimeResult execute_input(Context& exec_ctx);

		// 输入（以数字存储）
		RuntimeResult execute_input_num(Context& exec_ctx);

		// 清屏
		RuntimeResult execute_clear(Context& exec_ctx);

		// 判断变量是否为数字
		RuntimeResult execute_is_number(Context& exec_ctx);

		// 判断变量是否为字符串
		RuntimeResult execute_is_string(Context& exec_ctx);

		// 判断变量是否为列表
		RuntimeResult execute_is_list(Context& exec_ctx);

		// 判断变量是否为函数
		RuntimeResult execute_is_function(Context& exec_ctx);

		// 返回列表/字符串长度
		RuntimeResult execute_len(Context& exec_ctx);

		// 列表末尾追加元素(mutable)
		RuntimeResult execute_append(Context& exec_ctx);

		// 弹出指定位置的元素
		RuntimeResult execute_pop(Context& exec_ctx);

		// 弹出列表首部元素(mutable)
		RuntimeResult execute_pop_front(Context& exec_ctx);

		// 弹出列表尾部元素(mutable)
		RuntimeResult execute_pop_back(Context& exec_ctx);

		// 合并两个列表(mutable)
		RuntimeResult execute_extend(Context& exec_ctx);

	private:
		// 名称-函数对应
		static const map<string, function<RuntimeResult(BuiltInFunction*, Context&)>> func_name_map;

		// 名称-形参表对应
		static const map<string, vector<string>> func_args_map;
	};

}