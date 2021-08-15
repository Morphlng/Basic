#pragma once
#include <string>
using std::string;

namespace Basic {
	// Track line number, column number and current index
	// also the FileName, and FileContent
	class Position
	{

	public:
		Position(int idx = -1, int line = 0, int col = -1, string filename = "", string ftx = "");
		Position(const Position&);
		void advance(char current_char = '\0');

		int index;
		int row;
		int column;
		string fileName;
		string fileContent;
	};
}