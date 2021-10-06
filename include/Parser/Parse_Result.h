#pragma once
#include "Common/Error.h"
#include "Node.h"

namespace Basic
{

	// 该类将对语法分析的结果进行合法性检查
	class Parse_Result
	{
	public:
		Parse_Result(const shared_ptr<Error> &err = nullptr, const shared_ptr<ASTNode> &node = nullptr);
		Parse_Result(const Parse_Result &);

		shared_ptr<ASTNode> &registry(const Parse_Result &);
		shared_ptr<ASTNode> try_registry(const Parse_Result &);
		void registry_advancement();

		Parse_Result success(const shared_ptr<ASTNode> &);
		Parse_Result failure(const shared_ptr<Error> &);

		bool hasError();
		const shared_ptr<ASTNode> &getNode();
		const shared_ptr<Error> &getError();

	private:
		shared_ptr<Error> error;
		shared_ptr<ASTNode> node;
		int advance_count;

	public:
		int reverse_count;
	};

}