#include "../../include/Common/Position.h"

namespace Basic {

	Position::Position(int idx, int line, int col, const string& filename, const string& ftx)
	{
		this->index = idx;
		this->row = line;
		this->column = col;
		this->fileName = filename;
		this->fileContent = ftx;
	}

	Position::Position(const Position& other)
	{
		this->column = other.column;
		this->row = other.row;
		this->index = other.index;
		this->fileName = other.fileName;
		this->fileContent = other.fileContent;
	}

	void Position::advance(char current_char)
	{
		this->index++;
		this->column++;

		if (current_char == '\n')
		{
			this->row++;
			this->column = 0;
		}
	}

}