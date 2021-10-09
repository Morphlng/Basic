#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <regex>
#include <fstream>
#include <optional>
#include <algorithm>
#include <tuple>
#include "Error.h"
#include "crossline.h"
#include "Interpreter/Data.h"

using std::string;
using std::tuple;
using std::vector;

namespace Basic
{
	template <typename T>
	bool isIn(const vector<T> &list, T elem)
	{
		if (std::find(list.begin(), list.end(), elem) != list.end())
			return true;

		return false;
	}

	bool isLetter(char ch);

	bool isNumber(const string &token);

	string strip(const string &str, char ch = ' ');

	vector<string> split(const string &str, const string &regex);

	tuple<DataPtr, shared_ptr<Error>> run(const string &filename, const string &text);

	std::optional<string> readfile(const string &file_path);

	void printf(const char *s);

	template <typename T, typename... Args>
	void printf(const char *s, const T &value, const Args &...args)
	{
		while (*s)
		{
			if (*s == '%' && *++s != '%')
			{
				std::cout << value;
				return printf(++s, args...);
			}
			std::cout << *s++;
		}
		throw std::runtime_error("extra arguments provided to printf");
	}

	// 注意，string类型不能作为参数，请使用string.c_str()
	template <typename... Args>
	string format(const string &format, Args... args)
	{
		// Extra space for '\0'
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;

		if (size_s <= 0)
			throw std::runtime_error("Error during formatting.");

		size_t size = static_cast<size_t>(size_s);
		auto buf = std::make_unique<char[]>(size);
		std::snprintf(buf.get(), size, format.c_str(), args...);

		// We don't want the '\0' inside
		return string(buf.get(), buf.get() + size - 1);
	}

	// 以下函数用于crossline

	// auto-complete keyword / give hints
	struct keyword
	{
		const char *word;			// keyword
		crossline_color_e w_color;	// word_color
		const char *help;			// help
		crossline_color_e he_color; // help_color
		const char *hint;			// hint
		crossline_color_e hi_color; // hint_color

		keyword(const char *word = nullptr, crossline_color_e w_color = CROSSLINE_FGCOLOR_DEFAULT, const char *help = nullptr, crossline_color_e he_color = CROSSLINE_FGCOLOR_DEFAULT, const char *hint = nullptr, crossline_color_e hi_color = CROSSLINE_FGCOLOR_DEFAULT)
		{
			this->word = word;
			this->w_color = w_color;
			this->help = help;
			this->he_color = he_color;
			this->hint = hint;
			this->hi_color = hi_color;
		}
	};

	void completion_hook(char const *buf, crossline_completions_t *pCompletion);
}