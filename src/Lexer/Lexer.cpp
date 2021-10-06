#include "Common/utils.h"
#include "Lexer/Lexer.h"
#include <map>
using std::map;

namespace Basic
{
	Lexer::Lexer(const string &filename, const string &text)
	{
		this->filename = filename;
		this->text = text;
		this->pos = Position(-1, 0, -1, filename, text);
		this->current_char = '\0';
		advance();
	}

	void Lexer::advance()
	{
		this->pos.advance(this->current_char);
		if (this->pos.index < this->text.length())
			this->current_char = this->text[this->pos.index];
		else
			this->current_char = '\0';
	}

	void Lexer::skip_comment()
	{
		advance(); // 跳过#

		while (!Basic::isIn(NEWLINE, current_char)) // 不是换行或分号，一直步进
		{
			advance();
		}

		advance(); // 跳过换行
	}

	vector<Token> Lexer::make_tokens()
	{
		vector<Token> tokens;
		while (this->current_char != '\0')
		{
			if (Basic::isIn(IGNORES, current_char))
				advance();
			else if (this->current_char == '#') // 注释
			{
				skip_comment();
			}
			else if (Basic::isIn(NEWLINE, current_char))
			{
				tokens.push_back(Token(TD_NEWLINE, 0, this->pos));
				advance();
			}
			else if (this->current_char == ':')
			{
				tokens.push_back(Token(TD_COLON, 0, this->pos));
				advance();
			}
			else if (Basic::isLetter(current_char))
			{
				tokens.push_back(make_identifier());
			}
			else if (Basic::isIn(SIGNS, current_char))
			{
				tokens.push_back(make_sign());
			}
			else if (Basic::isIn(DIGITS, current_char))
			{
				tokens.push_back(make_number());
			}
			else
			{
				// Error occured
				Position start(this->pos);
				char error_char = this->current_char;
				advance();
				throw IllegalCharError(start, this->pos, "'" + string(1, error_char) + "'");
			}
		}

		tokens.push_back(Token(TD_EOF, 0, this->pos));
		return tokens;
	}

	Token Lexer::make_sign()
	{
		Token result;
		switch (this->current_char)
		{
		case '\"':
			return make_string();

		case '+':
			result = Token(TD_PLUS, 0, this->pos);
			advance();
			return result;

		case '-':
			return make_minus_or_arrow();

		case '*':
			result = Token(TD_MUL, 0, this->pos);
			advance();
			return result;

		case '/':
			result = Token(TD_DIV, 0, this->pos);
			advance();
			return result;

		case '^':
			result = Token(TD_POW, 0, this->pos);
			advance();
			return result;

		case '(':
			result = Token(TD_LPAREN, 0, this->pos);
			advance();
			return result;

		case ')':
			result = Token(TD_RPAREN, 0, this->pos);
			advance();
			return result;

		case '[':
			result = Token(TD_LSQUARE, 0, this->pos);
			advance();
			return result;

		case ']':
			result = Token(TD_RSQUARE, 0, this->pos);
			advance();
			return result;

		case '{':
			result = Token(TD_LBRACE, 0, this->pos);
			advance();
			return result;

		case '}':
			result = Token(TD_RBRACE, 0, this->pos);
			advance();
			return result;

		case ':':
			result = Token(TD_COLON, 0, this->pos);
			advance();
			return result;

		case '&':
			result = Token(TD_REF, 0, this->pos);
			advance();
			return result;

		case '!':
			return make_not_equals();

		case '=':
			return make_equals();

		case '<':
			return make_less_than();

		case '>':
			return make_greater_than();

		case '.':
			result = Token(TD_DOT, 0, this->pos);
			advance();
			return result;

		case ',':
			result = Token(TD_COMMA, 0, this->pos);
			advance();
			return result;

		default:
			return result;
		}
	}

	Token Lexer::make_number()
	{
		string num_str = "";
		bool isFloat = false;
		Position start = this->pos;

		while (this->current_char != '\0' && Basic::isIn(DIGITS, current_char))
		{
			if (this->current_char == '.')
			{ // 不能有两个小数点，所以遇到第二个退出
				if (isFloat)
					break;
				isFloat = true;
			}
			num_str += this->current_char;
			advance();
		}

		if (isFloat)
			return Token(TD_FLOAT, atof(num_str.c_str()), start, this->pos);
		else
			return Token(TD_INT, atoi(num_str.c_str()), start, this->pos);
	}

	Token Lexer::make_identifier()
	{
		string identifier;
		Position start = this->pos;

		while (!(this->current_char == '\0' || this->current_char == '.') && (Basic::isLetter(current_char) || Basic::isIn(DIGITS, current_char) || current_char == '_'))
		{
			identifier.push_back(current_char);
			advance();
		}

		string tok_type = TD_IDENTIFIER;
		if (Basic::isIn(KEYWORDS, identifier))
			tok_type = TD_KEYWORD;

		return Token(tok_type, identifier, start, this->pos);
	}

	Token Lexer::make_string()
	{
		string str;
		Position start = this->pos;
		advance();

		// 处理形如：str = "abc:\"xxx\""
		// 读取到\后的下一个字符需要无条件读入
		// 我们需要处理，如果\后面是n，则应为\n（换行）等情况
		bool escape = false; // 当遇到'\\'，设为true，意为接收下一个字符
		map<char, char> escape_characters{
			std::pair<char, char>('n', '\n'),
			std::pair<char, char>('t', '\t')}; // 上一个字符是\\，则下一个n应被转换为\n；以此类推

		while (current_char != '\0' && (current_char != '\"' || escape))
		{
			if (escape)
			{
				if (escape_characters.find(current_char) != escape_characters.end())
					str.push_back(escape_characters[current_char]);
				else
					str.push_back(current_char);
				escape = false;
			}
			else
			{
				if (current_char == '\\')
					escape = true;
				else
					str.push_back(current_char);
			}
			advance();
		}

		if (current_char != '\"')
			throw ExpectCharError(start, this->pos, "'\"' at the end of a string");

		advance();

		return Token(TD_STRING, str, start, this->pos);
	}

	Token Lexer::make_minus_or_arrow()
	{
		string tok_type = TD_MINUS;
		Position start = this->pos;
		advance();

		if (current_char == '>')
		{
			advance();
			tok_type = TD_ARROW;
		}

		return Token(tok_type, 0, start, this->pos);
	}

	Token Lexer::make_not_equals()
	{
		Position start = this->pos;
		advance();

		if (this->current_char == '=')
		{
			advance();
			return Token(TD_NE, 0, start, this->pos);
		}

		advance();
		throw ExpectCharError(start, this->pos, "'=' (after '!')");

		return Token();
	}

	Token Lexer::make_equals()
	{
		Position start = this->pos;
		string tok_type = TD_EQ;
		advance();

		if (this->current_char == '=')
		{
			advance();
			tok_type = TD_EE;
		}

		return Token(tok_type, 0, start, this->pos);
	}

	Token Lexer::make_less_than()
	{
		Position start = this->pos;
		string tok_type = TD_LT;
		advance();

		if (this->current_char == '=')
		{
			advance();
			tok_type = TD_LTE;
		}

		return Token(tok_type, 0, start, this->pos);
	}

	Token Lexer::make_greater_than()
	{
		Position start = this->pos;
		string tok_type = TD_GT;
		advance();

		if (this->current_char == '=')
		{
			advance();
			tok_type = TD_GTE;
		}

		return Token(tok_type, 0, start, this->pos);
	}
}