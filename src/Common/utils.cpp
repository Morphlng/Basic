#include "../../include/Common/utils.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

// auto-complete keyword / give hints
struct keyword
{
	const char* word;			// keyword
	crossline_color_e w_color;	// word_color
	const char* help;			// help
	crossline_color_e he_color; // help_color
	const char* hint;			// hint
	crossline_color_e hi_color; // hint_color

	keyword(const char* word = nullptr, crossline_color_e w_color = CROSSLINE_FGCOLOR_DEFAULT, const char* help = nullptr, crossline_color_e he_color = CROSSLINE_FGCOLOR_DEFAULT, const char* hint = nullptr, crossline_color_e hi_color = CROSSLINE_FGCOLOR_DEFAULT)
	{
		this->word = word;
		this->w_color = w_color;
		this->help = help;
		this->he_color = he_color;
		this->hint = hint;
		this->hi_color = hi_color;
	}
};

vector<keyword> keywords{
	{(const char*)"VAR", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_BLUE), (const char*)"Declare or Assign a variable", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"VAR IDENTIFIER = expr", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"IF", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"IF control statement", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"IF <condition> THEN <do sth>", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"THEN", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"THEN <do sth>", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"THEN <do sth>", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"ELIF", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"ELIF control statement", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"ELIF <condition> THEN <do sth>", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"ELSE", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"ELSE control statement", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"ELSE <do sth> END", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"WHILE", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"WHILE loop", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"WHILE <condition> THEN <do sth>", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"FOR", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"FOR loop", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"FOR <var>=<value> TO <end_value> STEP <step_value> THEN <do sth>", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"TO", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"TO for loop limit", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"TO <end_value>", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"STEP", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"STEP of each for loop", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"STEP <step_value>, default 1", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"BREAK", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"BREAK from loop", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"CONTINUE", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"Continue loop", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"RETURN", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_MAGENTA), (const char*)"RETURN from function", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"RETURN <variable/expression>", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"FUNC", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_BLUE), (const char*)"FUNC definition", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"FUNC (name)?(args)-> statement | FUNC (name)?(args): statements", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"END", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_BLUE), (const char*)"END of a multiline statements", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"AND", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"logic operator AND", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"OR", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"logic operator OR", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"NOT", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"logic operator NOT", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"null", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_BLUE), (const char*)"represent NULL", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"TRUE", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_BLUE), (const char*)"bool TRUE", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"FALSE", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_BLUE), (const char*)"bool FALSE", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"M_PI", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_BLUE), (const char*)"M_PI = 3.14159265354", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},

	// <built-in functions>
	{(const char*)"RUN", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"RUN a script from file", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"RUN(filepath), be aware of using / or \\\\", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"PRINT", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"PRINT a value", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"PRINT(value)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"PRINTS", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"PRINTS a list of value", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"PRINTS(list_of_value, ends_with), e.g PRINTS([1,\"2\",3.3],\"\\n\")", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"PRINT_RET", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"PRINT and RETURN a value", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"PRINT_RET(value)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"INPUT", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Receive user input as String", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"INPUT()", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"INPUT_NUM", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Receive user input as Number", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"INPUT_NUM()", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"CLEAR", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"CLEAR screen", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"CLEAR()", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"IS_NUM", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Check if given value is Number", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"IS_NUM(value)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"IS_STR", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Check if given value is String", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"IS_STR(value)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"IS_LIST", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Check if given value is List", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"IS_LIST(value)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"IS_FUNC", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Check if given value is Function", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"IS_FUNC(value)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"LEN", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Count the length of given List", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"LEN(list)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"APPEND", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"APPEND elem to list(mutable)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"APPEND(list, elem)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"POP", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Remove elem at given index in list(mutable)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"POP(list, index)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"POP_BACK", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Remove the last elem in list(mutable)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"POP_BACK(list)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"POP_FRONT", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Remove the first elem in list(mutable)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"POP_FRONT(list)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"EXTEND", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Concatenate list2 to list1(mutable)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"EXTEND(list1, list2)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{(const char*)"SWAP", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_YELLOW), (const char*)"Swap two variable, use & to pass reference", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN), (const char*)"SWAP(var1, var2)", (crossline_color_e)(CROSSLINE_FGCOLOR_BRIGHT | CROSSLINE_FGCOLOR_GREEN)},
	{nullptr, CROSSLINE_COLOR_DEFAULT, nullptr, CROSSLINE_COLOR_DEFAULT, nullptr, CROSSLINE_COLOR_DEFAULT} };

namespace Basic
{
	bool isLetter(char ch)
	{
		// A-Z || a-z
		if ((ch > 64 && ch < 91) || (ch > 96 && ch < 123))
			return true;
		return false;
	}

	bool isNumber(const string& token)
	{
		return std::regex_match(token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
	}

	string strip(const string& str, char ch)
	{
		// 除去str两端的ch字符
		int i = 0;
		size_t length = str.size();

		while (i < length && str[i] == ch) // 头部ch字符个数是i
			i++;
		if (i >= length)
			return "";

		int j = str.size() - 1;
		while (j >= 0 && str[j] == ch) // 尾部ch字符个数
			j--;
		return str.substr(i, j + 1 - i);
	}

	vector<string> split(const string& str, const string& regex_str)
	{
		std::regex regexz(regex_str);
		std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
			std::sregex_token_iterator());
		return list;
	}

	void printf(const char* s)
	{
		while (*s)
		{
			if (*s == '%' && *++s != '%')
				throw std::runtime_error("invalid format string: missing arguments");
			std::cout << *s++;
		}
	}

	void completion_hook(const char* buf, crossline_completions_t* pCompletion)
	{
		string last_input = split(string(buf), "[ ,\(]").back();

		for (keyword& p : keywords)
		{
			if (p.word == nullptr)
				break;

			if (strncmp(last_input.c_str(), p.word, strlen(last_input.c_str())) == 0)
			{
				// 如果全字匹配，则显示提示
				if (last_input == p.word)
				{
					if (p.hint != nullptr)
						crossline_hints_set_color(pCompletion, p.hint, p.hi_color);
					break;
				}
				// 否则去找匹配项
				crossline_completion_add_color(pCompletion, p.word, p.w_color, p.help, p.he_color);
			}
		}
	}
}