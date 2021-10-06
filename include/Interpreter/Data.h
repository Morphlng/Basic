#pragma once

#include <memory>
#include <functional>
#include <map>
#include "Common/Position.h"
#include "Common/Context.h"
#include "Parser/Node.h"
#include "RuntimeResult.h"
#include "RunTimeError.h"

using std::function;
using std::make_shared;
using std::make_unique;
using std::map;
using std::pair;
using std::shared_ptr;
using std::unique_ptr;

namespace Basic
{
	using DataPtr = shared_ptr<unique_ptr<Data>>;

	template <class T, typename... Args>
	DataPtr make_Dataptr(const Args &...args)
	{
		static_assert(std::is_base_of<Data, T>::value, "T must inherit from Data");
		return make_shared<unique_ptr<Data>>(make_unique<T>(args...));
	}

	// 注意，传入的ptr不可以是右值（临时变量），否则T*和ptr会在函数结束后一同销毁
	template <class T>
	T *raw_Dataptr(DataPtr ptr)
	{
		return static_cast<T *>((*ptr).get());
	}

	class Data
	{
	public:
		void set_pos(const Position &start, const Position &end);
		void set_context(Context *);

		virtual ~Data()
		{
			context = nullptr;
		}

		virtual DataPtr clone()
		{
			return make_Dataptr<Data>(*this);
		}

		// 算数运算
		virtual DataPtr added_to(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr subbed_by(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr multed_by(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr dived_by(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr powed_by(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}

		// 布尔运算
		virtual DataPtr get_comparison_eq(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr get_comparison_ne(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr get_comparison_lt(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr get_comparison_gt(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr get_comparison_lte(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr get_comparison_gte(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}

		// 逻辑运算
		virtual DataPtr anded_by(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr ored_by(const DataPtr &other)
		{
			illegal_operation(other);
			return nullptr;
		}
		virtual DataPtr notted()
		{
			illegal_operation();
			return nullptr;
		}

		// Callable
		virtual RuntimeResult execute(vector<DataPtr> &args)
		{
			return RuntimeResult().failure(make_shared<RunTimeError>(this->pos_start, this->pos_end, "Variable is not callable", *this->context));
		}

		// 下标索引
		virtual DataPtr index_by(const DataPtr &other)
		{
			illegal_operation();
			return nullptr;
		}

		// 取属性('.')
		virtual DataPtr attr_by(const Token &attribute)
		{
			illegal_operation();
			return nullptr;
		}

		virtual bool is_true() { return false; }
		virtual string repr() { return "undefined"; }

		void illegal_operation(const DataPtr &other = nullptr)
		{
			if (other == nullptr)
				throw RunTimeError(this->pos_start, this->pos_end, "Illegal operation", *this->context);
			else
				throw RunTimeError(this->pos_start, (*other)->pos_end, "Illegal operation", *this->context);
		}

	public:
		Position pos_start;
		Position pos_end;
		Context *context;
	};

	class Number : public Data
	{
	public:
		Number(double value = 0, const Position &start = Position(), const Position &end = Position(), Context * = nullptr);
		Number(const Number &);
		~Number() {}
		double get_value(bool wantInt = false);

		DataPtr clone() override;
		DataPtr added_to(const DataPtr &other) override;
		DataPtr subbed_by(const DataPtr &other) override;
		DataPtr multed_by(const DataPtr &other) override;
		DataPtr dived_by(const DataPtr &other) override;
		DataPtr powed_by(const DataPtr &other) override;

		DataPtr get_comparison_eq(const DataPtr &other) override;
		DataPtr get_comparison_ne(const DataPtr &other) override;
		DataPtr get_comparison_lt(const DataPtr &other) override;
		DataPtr get_comparison_gt(const DataPtr &other) override;
		DataPtr get_comparison_lte(const DataPtr &other) override;
		DataPtr get_comparison_gte(const DataPtr &other) override;

		DataPtr anded_by(const DataPtr &other) override;
		DataPtr ored_by(const DataPtr &other) override;
		DataPtr notted() override;

		// Number类同样掌管bool判断，所以有判断是否为真的功能
		bool is_true() override;

		string repr() override;

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
		String(const String &);
		~String()
		{
			value.clear();
		}

		DataPtr clone() override;
		DataPtr added_to(const DataPtr &) override;
		DataPtr multed_by(const DataPtr &) override;

		DataPtr index_by(const DataPtr &) override;

		DataPtr get_comparison_eq(const DataPtr &other) override;
		DataPtr get_comparison_ne(const DataPtr &other) override;

		bool is_true() override;
		string repr() override;
		string str(); // Print时不希望带有引号
		string getValue();

	private:
		string value;
	};

	class List : public Data
	{
	public:
		List(const vector<DataPtr> &elems);
		List(const List &);
		~List()
		{
			elements.clear();
		}

		DataPtr clone() override;

		// add new elem
		DataPtr added_to(const DataPtr &) override;

		// combine two list
		DataPtr multed_by(const DataPtr &) override;

		// remove elem of the given index(Number)
		DataPtr subbed_by(const DataPtr &) override;

		// get elem of given index(Number)
		DataPtr index_by(const DataPtr &) override;

		DataPtr get_comparison_eq(const DataPtr &other) override;
		DataPtr get_comparison_ne(const DataPtr &other) override;

		string repr() override;

		vector<DataPtr> &get_elements();

	private:
		vector<DataPtr> elements;
	};

	class Dict : public Data
	{
	public:
		Dict(const map<string, DataPtr> &elem);
		Dict(const Dict &other);
		~Dict()
		{
			elements.clear();
		}

		// get elem of given name(String)
		DataPtr index_by(const DataPtr &) override;

		DataPtr attr_by(const Token &attribute) override;

		DataPtr clone() override;
		string repr() override;

	private:
		map<string, DataPtr> elements;
	};

	// 函数类的基类，封装了公有行为
	class BaseFunction : public Data
	{
	public:
		BaseFunction(const string &func_name);
		BaseFunction(const BaseFunction &other);
		~BaseFunction()
		{
			func_name.clear();
		}

		DataPtr clone() override;

		// 生成函数的“上下文”
		Context generate_new_context();

		// 检查参数个数是否匹配，若匹配则加入到Context中
		RuntimeResult check_populate_args(const vector<string> &arg_names, vector<DataPtr> &args, Context &exec_ctx);

		string repr() override;

	protected:
		string func_name;

	private:
		// 检查参数个数是否匹配
		RuntimeResult check_args(const vector<string> &arg_names, const vector<DataPtr> &args);

		// 将<参数-值>对，加入到Symbol_Table中
		void populate_args(const vector<string> &arg_names, vector<DataPtr> &args, Context &exec_ctx);
	};

	class Function : public BaseFunction
	{
	public:
		Function(const string &func_name, const shared_ptr<ASTNode> &body_node, const vector<string> &arg_names, bool auto_return = true);
		Function(const Function &);
		~Function()
		{
			body_node.reset();
			arg_names.clear();
		}
		DataPtr clone() override;

		RuntimeResult execute(vector<DataPtr> &args) override;

	private:
		shared_ptr<ASTNode> body_node;
		vector<string> arg_names;
		bool auto_return;
	};

	class BuiltInFunction : public BaseFunction
	{
	public:
		BuiltInFunction(const string &func_name);
		BuiltInFunction(const BuiltInFunction &);
		~BuiltInFunction() {}

		string repr() override;
		DataPtr clone() override;

		RuntimeResult execute(vector<DataPtr> &args) override;

		// 内置函数

		// 运行外部文件
		RuntimeResult execute_run(Context &exec_ctx);

		// 输出value
		RuntimeResult execute_print(Context &exec_ctx);

		// 将传入的列表中的元素以空格拼接，输出
		RuntimeResult execute_prints(Context &exec_ctx);

		// 返回value
		RuntimeResult execute_print_ret(Context &exec_ctx);

		// 输入（以字符串存储）
		RuntimeResult execute_input(Context &exec_ctx);

		// 输入（以数字存储）
		RuntimeResult execute_input_num(Context &exec_ctx);

		// 清屏
		RuntimeResult execute_clear(Context &exec_ctx);

		// 判断变量是否为数字
		RuntimeResult execute_is_number(Context &exec_ctx);

		// 判断变量是否为字符串
		RuntimeResult execute_is_string(Context &exec_ctx);

		// 判断变量是否为列表
		RuntimeResult execute_is_list(Context &exec_ctx);

		// 判断变量是否为函数
		RuntimeResult execute_is_function(Context &exec_ctx);

		// 返回列表/字符串长度
		RuntimeResult execute_len(Context &exec_ctx);

		// 列表末尾追加元素(mutable)
		RuntimeResult execute_append(Context &exec_ctx);

		// 弹出指定位置的元素
		RuntimeResult execute_pop(Context &exec_ctx);

		// 弹出列表首部元素(mutable)
		RuntimeResult execute_pop_front(Context &exec_ctx);

		// 弹出列表尾部元素(mutable)
		RuntimeResult execute_pop_back(Context &exec_ctx);

		// 合并两个列表(mutable)
		RuntimeResult execute_extend(Context &exec_ctx);

		// 交换两个变量
		RuntimeResult execute_swap(Context &exec_ctx);

	private:
		// 名称-函数对应
		static const map<string, function<RuntimeResult(BuiltInFunction *, Context &)>> func_name_map;

		// 名称-形参表对应
		static const map<string, vector<string>> func_args_map;
	};
}