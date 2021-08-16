#pragma once

#include <memory>
#include "../Common/Error.h"

using std::make_shared;
using std::shared_ptr;

namespace Basic {

	class Data;

	// 类似于ParseResult，该类将保管解释器的内容，并提供异常处理
	class RuntimeResult
	{
	public:
		RuntimeResult(shared_ptr<Data> _value = nullptr, shared_ptr<Error> err = nullptr, shared_ptr<Data> return_value = nullptr);
		RuntimeResult(const RuntimeResult&);
		void reset();

		shared_ptr<Data> registry(const RuntimeResult& res);

		// succuess for value
		RuntimeResult success(const shared_ptr<Data>& value);
		// succuess for error
		RuntimeResult failure(const shared_ptr<Error>& err);
		// succuess for return
		RuntimeResult success_return(const shared_ptr<Data>& return_value);
		// succuess for loop_continue
		RuntimeResult success_continue();
		// succuess for loop_break
		RuntimeResult success_break();

		bool hasError();

		// 当出现Error、Return、Continue或Break时，提前终止
		bool should_return();

		const shared_ptr<Data>& getValuePtr();
		const shared_ptr<Error>& getError();
		const shared_ptr<Data>& get_func_return_value();
		bool should_continue();
		bool should_break();

	private:
		// 用于记录函数返回值
		// 若函数被Call了，那么将该值赋给value
		shared_ptr<Data> func_return_value;

		// value用于赋值给一个变量
		shared_ptr<Data> value;
		shared_ptr<Error> error;

		// 记录一次循环是否需要跳过
		bool loop_continue;

		// 记录是否需要跳出当前循环
		bool loop_break;
	};
}