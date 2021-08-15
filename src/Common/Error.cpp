#include "../../include/Common/Error.h"
#include "../../include/Common/utils.h"
#include <algorithm>

namespace Basic {

	string string_with_arrows(string content, Position start, Position end)
	{
		string result = "";

		// indices
		int idx_start = content.rfind("\n", start.index);
		idx_start = (idx_start == string::npos) ? 0 : idx_start;
		int idx_end = content.find('\n', idx_start + 1);

		// Generate Line
		int line_count = end.row - start.row + 1;
		for (int i = 0; i < line_count; i++)
		{
			string line = content.substr(idx_start, idx_end - idx_start);

			// calculate line column
			int col_start = i == 0 ? start.column : 0;
			int col_end = (i == line_count - 1) ? end.column : line.length() - 1;

			// append to result
			result += line + "\n";

			for (int j = 0; j < col_start; j++)
				result += " ";

			for (int j = 0; j < (col_end - col_start); j++)
				result += "^";

			// Recalculate indices
			idx_start = idx_end;
			idx_end = content.find('\n', idx_start + 1);
			idx_end = (idx_end == string::npos) ? content.length() : idx_end;
		}

		for (auto it = result.begin(); it != result.end();)
		{
			if ((*it) == '\t')
				it = result.erase(it);
			else
				it++;
		}

		return result;
	}

	Error::Error(const Position& start, const Position& end, const string& name, const string& details)
	{
		this->pos_start = start;
		this->pos_end = end;
		this->error_name = name;
		this->details = details;
	}

	string Error::as_string()
	{
		string result = Basic::format("%s: %s\n", this->error_name.c_str(), this->details.c_str());
		result += Basic::format("File %s, line %s\n\n", this->pos_start.fileName.c_str(), std::to_string(this->pos_start.row + 1).c_str());
		result += string_with_arrows(pos_start.fileContent, pos_start, pos_end);

		return result;
	}

}