#pragma once

#include "../Common/Context.h"
#include "../Parser/Node.h"
#include "RuntimeResult.h"
#include "Data.h"

namespace Basic {

	class Interpreter
	{
	public:
		RuntimeResult visit(shared_ptr<ASTNode> root, Context&);

	private:
		RuntimeResult visit_NumberNode(shared_ptr<NumberNode> root, Context&);
		RuntimeResult visit_StringNode(shared_ptr<StringNode> root, Context&);
		RuntimeResult visit_ListNode(shared_ptr<ListNode> root, Context&);

		RuntimeResult visit_BinOpNode(shared_ptr<BinOpNode> root, Context&);
		RuntimeResult visit_UnaryOpNode(shared_ptr<UnaryOpNode> root, Context&);

		RuntimeResult visit_VarAccessNode(shared_ptr<VarAccessNode> root, Context&);
		RuntimeResult visit_VarAssignNode(shared_ptr<VarAssignNode> root, Context&);
		RuntimeResult visit_MutateNode(shared_ptr<MutateNode> root, Context&);
		RuntimeResult visit_IndexNode(shared_ptr<IndexNode> root, Context&);

		RuntimeResult visit_IfNode(shared_ptr<IfNode> root, Context&);
		RuntimeResult visit_ForNode(shared_ptr<ForNode> root, Context&);
		RuntimeResult visit_WhileNode(shared_ptr<WhileNode> root, Context&);

		RuntimeResult visit_FuncDefNode(shared_ptr<FuncDefNode> root, Context&);
		RuntimeResult visit_CallNode(shared_ptr<CallNode> root, Context&);
		RuntimeResult visit_ReturnNode(shared_ptr<ReturnNode> root, Context&);
		RuntimeResult visit_BreakNode(shared_ptr<BreakNode> root, Context&);
		RuntimeResult visit_ContinueNode(shared_ptr<ContinueNode> root, Context&);
	};

}