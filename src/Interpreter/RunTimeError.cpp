#include "../../include/Interpreter/RunTimeError.h"

namespace Basic {

	RunTimeError::RunTimeError(const Position& start, const Position& end, const string& details, Context& context) : Error(start, end, "Runtime Error", details)
	{
		this->context = context;
	}

	string RunTimeError::as_string()
	{
		string result = generate_traceback();
		result += this->error_name + ": " + this->details;
		result += "\n\n\n" + string_with_arrows(pos_start.fileContent, pos_start, pos_end);

		return result;
	}

	string RunTimeError::generate_traceback()
	{
		string result = "";
		Position pos = this->pos_start;
		shared_ptr<Context> ctx = make_shared<Context>(this->context);

		while (ctx != nullptr)
		{
			result = "  File " + pos.fileName + ", line " + std::to_string(pos.row + 1) + ", in " + ctx->get_displayName() + "\n" + result;
			pos = ctx->get_parent_entry_pos();
			ctx = ctx->get_parent();
		}

		return "Traceback (most recent call last):\n" + result;
	}
}