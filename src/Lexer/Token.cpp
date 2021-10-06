#include "Lexer/Token.h"

namespace Basic
{
	Token::Token(const string &type_, const string &value_, const Position &start, const Position &end)
	{
		this->type = type_;
		this->value = value_;

		if (start.index != -1)
		{
			this->pos_start = start;
			this->pos_end = start;
			this->pos_end.advance();
		}

		if (end.index != -1)
			this->pos_end = end;
	}

	Token::Token(const string &type_, double value_, const Position &start, const Position &end)
	{
		this->type = type_;

		if (value_ == 0 && type_ != TD_INT)
			this->value = "0";
		else
			this->value = std::to_string(value_);

		if (start.index != -1)
		{
			this->pos_start = start;
			this->pos_end = start;
			this->pos_end.advance();
		}

		if (end.index != -1)
			this->pos_end = end;
	}

	Token::Token(const Token &other)
	{
		this->type = other.type;
		this->value = other.value;
		this->pos_start = other.pos_start;
		this->pos_end = other.pos_end;
	}

	bool Token::operator==(const Token &other) const
	{
		if (this->type != other.type)
			return false;
		if (this->value != other.value)
			return false;

		return true;
	}

	bool Token::matches(const string &_type, const string &_value)
	{
		if (this->type == _type && this->value == _value)
			return true;

		return false;
	}

	void Token::set_pos(const Position &start, const Position &end)
	{
		this->pos_start = start;
		this->pos_end = end;
	}

	double Token::get_number()
	{
		if (type == TD_INT)
			return atoi(value.c_str());
		else if (type == TD_FLOAT)
			return atof(value.c_str());

		return -1;
	}

	string Token::repr()
	{
		if (this->value == "0" && this->type != TD_INT)
			return this->type;

		if (this->type == TD_INT)
			return this->type + ":" + std::to_string(atoi(this->value.c_str()));
		else if (this->type == TD_FLOAT)
			return this->type + ":" + std::to_string(atof(this->value.c_str()));
		else
			return this->type + ":" + this->value;
	}
}