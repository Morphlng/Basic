#pragma once
#include "../Common/Error.h"
#include "../Common/Context.h"

namespace Basic {

	class Context;

	class RunTimeError : public Error
	{
	public:
		RunTimeError(const Position& start, const Position& end, const string& details, Context& context);
		string as_string() override;
		string generate_traceback();

	private:
		Context context;
	};
}