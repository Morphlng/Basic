#pragma once
#include <vector>
#include <string>

using std::string;
using std::vector;

// -------常量定义-------

// -------符号定义-------
const string TD_INT = "INT";
const string TD_FLOAT = "FLOAT";
const string TD_STRING = "STRING";
const string TD_PLUS = "PLUS";
const string TD_MINUS = "MINUS";
const string TD_MUL = "MUL";
const string TD_DIV = "DIV";
const string TD_POW = "POW";
const string TD_DOT = "DOT"; // '.' 用于字典对象
const string TD_LPAREN = "LPAREN";
const string TD_RPAREN = "RPAREN";
const string TD_LSQUARE = "LSQUARE"; // [
const string TD_RSQUARE = "RSQUARE"; // ]
const string TD_LBRACE = "LBRACE";	 // {
const string TD_RBRACE = "RBRACE";	 // }

const string TD_REF = "REF";		 // &，引用符号
const string TD_COLON = "COLON";	 // ':'
const string TD_NEWLINE = "NEWLINE"; // 允许多行输入
const string TD_EOF = "EOF";		 // 词法分析终止符

// -------布尔运算-------
const string TD_EE = "EE";	 // equal "=="
const string TD_NE = "NE";	 // not equal "!="
const string TD_LT = "LT";	 // less than "<"
const string TD_GT = "GT";	 // greater than ">"
const string TD_LTE = "LTE"; // less than equal "<="
const string TD_GTE = "GTE"; // greater than equal ">="

// -------变量-------
const string TD_KEYWORD = "KEYWORD";
const string TD_IDENTIFIER = "IDENTIFIER";
const string TD_EQ = "EQ"; // assign "="

// -------函数-------
const string TD_COMMA = "COMMA"; // ,
const string TD_ARROW = "ARROW"; // ->

// 词法分析常量

const vector<char> SIGNS{'+', '-', '*', '/', '(', ')', '[', ']', '{', '}', ':', '^', '=', '!', '<', '>', ',', '.', '\"', '&'};
const vector<char> DIGITS{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const vector<char> IGNORES{' ', '\r', '\t'};
const vector<char> NEWLINE{'\n', ';', '\0'};
const vector<string> KEYWORDS{
	"VAR",		// 声明变量
	"AND",		// 与运算
	"OR",		// 或运算
	"NOT",		// 非运算
	"IF",		// IF <condition>
	"THEN",		// THEN <expr>
	"ELIF",		// ELIF <condition>
	"ELSE",		// ELSE <expr>
	"FOR",		// FOR loop
	"TO",		// FOR end_value
	"STEP",		// FOR increment step
	"WHILE",	// WHILE loop
	"FUNC",		// FUNCTION define
	"END",		// END of a Multi-line statement
	"RETURN",	// Return From function
	"CONTINUE", // Continue loop
	"BREAK",	// Break from loop
	"DEL"		// Delete a variable
};