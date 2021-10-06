#pragma once

#include "Common/Context.h"
#include "Parser/Node.h"
#include "RuntimeResult.h"
#include "Data.h"

namespace Basic
{
	class Interpreter
	{
	public:
		RuntimeResult visit(const shared_ptr<ASTNode>& root, Context&, bool = false);

	private:
		RuntimeResult visit_NumberNode(const shared_ptr<NumberNode>& root, Context&);
		RuntimeResult visit_StringNode(const shared_ptr<StringNode>& root, Context&);
		RuntimeResult visit_ListNode(const shared_ptr<ListNode>& root, Context&);
		RuntimeResult visit_DictNode(const shared_ptr<DictNode>& root, Context&);

		RuntimeResult visit_BinOpNode(const shared_ptr<BinOpNode>& root, Context&);
		RuntimeResult visit_UnaryOpNode(const shared_ptr<UnaryOpNode>& root, Context&);

		RuntimeResult visit_VarAccessNode(const shared_ptr<VarAccessNode>& root, Context&, bool byRef);
		RuntimeResult visit_VarAssignNode(const shared_ptr<VarAssignNode>& root, Context&);
		RuntimeResult visit_VarDeleteNode(const shared_ptr<VarDeleteNode>& root, Context&);
		RuntimeResult visit_MutateNode(const shared_ptr<MutateNode>& root, Context&);
		RuntimeResult visit_IndexNode(const shared_ptr<IndexNode>& root, Context&);
		RuntimeResult visit_AttrNode(const shared_ptr<AttrNode>& root, Context&);

		RuntimeResult visit_IfNode(const shared_ptr<IfNode>& root, Context&);
		RuntimeResult visit_ForNode(const shared_ptr<ForNode>& root, Context&);
		RuntimeResult visit_WhileNode(const shared_ptr<WhileNode>& root, Context&);

		RuntimeResult visit_FuncDefNode(const shared_ptr<FuncDefNode>& root, Context&);
		RuntimeResult visit_CallNode(const shared_ptr<CallNode>& root, Context&);
		RuntimeResult visit_ReturnNode(const shared_ptr<ReturnNode>& root, Context&);
		RuntimeResult visit_BreakNode(const shared_ptr<BreakNode>& root, Context&);
		RuntimeResult visit_ContinueNode(const shared_ptr<ContinueNode>& root, Context&);
	};
}