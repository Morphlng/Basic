#include "../../include/Interpreter/RuntimeResult.h"

namespace Basic {

	RuntimeResult::RuntimeResult(shared_ptr<Data> _value, shared_ptr<Error> err, shared_ptr<Data> return_value)
	{
		this->value = _value;
		this->error = err;
		this->func_return_value = return_value;
		this->loop_break = false;
		this->loop_continue = false;
	}

	RuntimeResult::RuntimeResult(const RuntimeResult& other)
	{
		this->value = other.value;
		this->error = other.error;
		this->func_return_value = other.func_return_value;
		this->loop_continue = other.loop_continue;
		this->loop_break = other.loop_break;
	}

	void RuntimeResult::reset()
	{
		this->func_return_value = nullptr;
		this->value = nullptr;
		this->error = nullptr;
		this->loop_break = false;
		this->loop_continue = false;
	}

	shared_ptr<Data> RuntimeResult::registry(const RuntimeResult& res)
	{
		this->error = res.error;
		this->func_return_value = res.func_return_value;
		this->loop_break = res.loop_break;
		this->loop_continue = res.loop_continue;

		return res.value;
	}

	RuntimeResult RuntimeResult::success(const shared_ptr<Data>& value)
	{
		reset();
		this->value = value;
		return (*this);
	}

	RuntimeResult RuntimeResult::failure(const shared_ptr<Error>& err)
	{
		reset();
		this->error = err;
		return (*this);
	}

	RuntimeResult RuntimeResult::success_return(const shared_ptr<Data>& return_value)
	{
		reset();
		this->func_return_value = return_value;
		return (*this);
	}

	RuntimeResult RuntimeResult::success_continue()
	{
		reset();
		this->loop_continue = true;
		return (*this);
	}

	RuntimeResult RuntimeResult::success_break()
	{
		reset();
		this->loop_break = true;
		return (*this);
	}

	bool RuntimeResult::hasError()
	{
		return error != nullptr;
	}

	bool RuntimeResult::should_return()
	{
		return (error != nullptr || func_return_value != nullptr || loop_continue || loop_break);
	}

	const shared_ptr<Data>& RuntimeResult::getValuePtr()
	{
		return this->value;
	}

	const shared_ptr<Error>& RuntimeResult::getError()
	{
		return this->error;
	}

	const shared_ptr<Data>& RuntimeResult::get_func_return_value()
	{
		return this->func_return_value;
	}

	bool RuntimeResult::should_continue()
	{
		return this->loop_continue;
	}

	bool RuntimeResult::should_break()
	{
		return this->loop_break;
	}
}