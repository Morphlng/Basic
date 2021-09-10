#include "../../include/Parser/Parse_Result.h"

namespace Basic
{

	Parse_Result::Parse_Result(const shared_ptr<Error> &err, const shared_ptr<ASTNode> &node)
	{
		this->error = err;
		this->node = node;
		this->advance_count = 0;
		this->reverse_count = 0;
	}

	Parse_Result::Parse_Result(const Parse_Result &other)
	{
		this->error = other.error;
		this->node = other.node;
		this->advance_count = other.advance_count;
		this->reverse_count = other.reverse_count;
	}

	shared_ptr<ASTNode> &Parse_Result::registry(const Parse_Result &res)
	{
		this->advance_count += res.advance_count;
		this->error = res.error;
		this->node = res.node;

		return this->node;
	}

	shared_ptr<ASTNode> Parse_Result::try_registry(const Parse_Result &res)
	{
		if (res.error != nullptr)
		{
			this->reverse_count = res.advance_count;
			return nullptr;
		}
		return registry(res);
	}

	void Parse_Result::registry_advancement()
	{
		this->advance_count++;
	}

	Parse_Result Parse_Result::success(const shared_ptr<ASTNode> &node)
	{
		this->node = node;
		return (*this);
	}

	Parse_Result Parse_Result::failure(const shared_ptr<Error> &err)
	{
		if (this->error == nullptr || advance_count == 0)
			this->error = err;
		return (*this);
	}

	bool Parse_Result::hasError()
	{
		if (this->error != nullptr)
			return true;

		return false;
	}

	const shared_ptr<ASTNode> &Parse_Result::getNode()
	{
		return this->node;
	}

	const shared_ptr<Error> &Parse_Result::getError()
	{
		return this->error;
	}

}