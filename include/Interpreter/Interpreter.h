#pragma once

#include "../Common/Context.h"
#include "../Parser/Node.h"
#include "RuntimeResult.h"
#include "Data.h"

namespace Basic
{
	class Interpreter
	{
	public:
		RuntimeResult visit(ASTNode *root, Context &, bool byRef = false);

	private:
		RuntimeResult visit_NumberNode(NumberNode *root, Context &);
		RuntimeResult visit_StringNode(StringNode *root, Context &);
		RuntimeResult visit_ListNode(ListNode *root, Context &);

		RuntimeResult visit_BinOpNode(BinOpNode *root, Context &);
		RuntimeResult visit_UnaryOpNode(UnaryOpNode *root, Context &);

		RuntimeResult visit_VarAccessNode(VarAccessNode *root, Context &, bool byRef = false);
		RuntimeResult visit_VarAssignNode(VarAssignNode *root, Context &);
		RuntimeResult visit_MutateNode(MutateNode *root, Context &);
		RuntimeResult visit_IndexNode(IndexNode *root, Context &);

		RuntimeResult visit_IfNode(IfNode *root, Context &);
		RuntimeResult visit_ForNode(ForNode *root, Context &);
		RuntimeResult visit_WhileNode(WhileNode *root, Context &);

		RuntimeResult visit_FuncDefNode(FuncDefNode *root, Context &);
		RuntimeResult visit_CallNode(CallNode *root, Context &);
		RuntimeResult visit_ReturnNode(ReturnNode *root, Context &);
		RuntimeResult visit_BreakNode(BreakNode *root, Context &);
		RuntimeResult visit_ContinueNode(ContinueNode *root, Context &);
	};
}