#include "../include/Common/utils.h"
#include "../include/Common/Context.h"
#include "../include/Lexer/Lexer.h"
#include "../include/Parser/Parser.h"
#include "../include/Interpreter/Interpreter.h"

// #define DEBUG

using namespace std;
using namespace Basic;

SymbolTable global_symbol_table;
Context context("<program>");

tuple<shared_ptr<Data>, shared_ptr<Error>> Basic::run(string filename, string text)
{
	// LexicalAnalysis
	Lexer lexer(filename, text);
	vector<Token> lex_result;
	try
	{
		lex_result = lexer.make_tokens();
#ifdef DEBUG
		cout << "\n[";
		for (Token t : lex_result)
		{
			cout << t.__repr__() << ", ";
		}
		cout << "]" << endl;
#endif
	}
	catch (Error &e)
	{
		return make_tuple(nullptr, make_shared<Error>(e));
	}

	// 当为注释时，仅有EOF
	if (lex_result.size() == 1)
		return make_tuple(nullptr, nullptr);

	// Parsing
	Parser parse(lex_result);
	Parse_Result res = parse.parse();
	shared_ptr<ASTNode> root = res.getNode();
	shared_ptr<Error> err = res.getError();

	if (err != nullptr)
	{
		return make_tuple(nullptr, err);
	}
#ifdef DEBUG
	else
	{
		cout << root->__repr__() << endl;
	}
#endif

	// Interpret
	Interpreter interpreter;

	RuntimeResult interprete_result = interpreter.visit(root, context);

	shared_ptr<Data> data = interprete_result.getValuePtr();
	err = interprete_result.getError();

	if (err != nullptr)
	{
		return make_tuple(nullptr, err);
	}
	else if (data != nullptr && typeid(*data) != typeid(Data))
	{
		return make_tuple(data, nullptr);
	}

	return make_tuple(nullptr, nullptr);
}

void Init()
{
	// 常量定义
	global_symbol_table.set("null", make_shared<Number>(Number::null));
	global_symbol_table.set("TRUE", make_shared<Number>(Number::TRUE));
	global_symbol_table.set("FALSE", make_shared<Number>(Number::FALSE));
	global_symbol_table.set("M_PI", make_shared<Number>(Number::MATH_PI));

	// 内置函数
	global_symbol_table.set("RUN", make_shared<BuiltInFunction>("RUN"));
	global_symbol_table.set("PRINT", make_shared<BuiltInFunction>("PRINT"));
	global_symbol_table.set("PRINTS", make_shared<BuiltInFunction>("PRINTS"));
	global_symbol_table.set("PRINT_RET", make_shared<BuiltInFunction>("PRINT_RET"));

	global_symbol_table.set("INPUT", make_shared<BuiltInFunction>("INPUT"));
	global_symbol_table.set("INPUT_NUM", make_shared<BuiltInFunction>("INPUT_NUM"));
	global_symbol_table.set("CLEAR", make_shared<BuiltInFunction>("CLEAR"));

	global_symbol_table.set("IS_NUM", make_shared<BuiltInFunction>("IS_NUM"));
	global_symbol_table.set("IS_STR", make_shared<BuiltInFunction>("IS_STR"));
	global_symbol_table.set("IS_LIST", make_shared<BuiltInFunction>("IS_LIST"));
	global_symbol_table.set("IS_FUNC", make_shared<BuiltInFunction>("IS_FUNC"));

	global_symbol_table.set("LEN", make_shared<BuiltInFunction>("LEN"));
	global_symbol_table.set("APPEND", make_shared<BuiltInFunction>("APPEND"));
	global_symbol_table.set("POP", make_shared<BuiltInFunction>("POP"));
	global_symbol_table.set("POP_BACK", make_shared<BuiltInFunction>("POP_BACK"));
	global_symbol_table.set("POP_FRONT", make_shared<BuiltInFunction>("POP_FRONT"));
	global_symbol_table.set("EXTEND", make_shared<BuiltInFunction>("EXTEND"));

	context.set_symbol_table(global_symbol_table);

	crossline_completion_register(Basic::completion_hook);
	crossline_history_load("history.txt");
	crossline_prompt_color_set(CROSSLINE_FGCOLOR_CYAN);

	cout << "Initialize complete, welcome to Basic\n\n";
}

int main()
{
	Init();

	char line[256];
	while (crossline_readline("basic > ", line, sizeof(line)) && (strncmp(line, "exit", 4)))
	{
		string text;
		string tmp = Basic::strip(string(line));
		if (tmp == "")
			continue;
		else
		{
			text.append(tmp);

			while ((*tmp.rbegin()) == ';' || (*tmp.rbegin()) == ':')
			{
				crossline_readline("...     ", line, sizeof(line));
				tmp = Basic::strip(string(line));
				if (tmp == "")
					break;

				text.append(tmp);
			}

			auto result = Basic::run("<stdin>", text);
			if (std::get<1>(result) != nullptr)
			{
				cout << std::get<1>(result)->as_string() << "\n";
			}
			else if (std::get<0>(result) != nullptr)
			{
				cout << std::get<0>(result)->__repr__() << "\n";
			}
		}
		cout << endl;
	}

	crossline_history_save("history.txt");
	return 0;
}