#include "../include/Common/utils.h"
#include "../include/Common/Context.h"
#include "../include/Lexer/Lexer.h"
#include "../include/Parser/Parser.h"
#include "../include/Interpreter/Interpreter.h"

// #define DEBUG

using namespace std;
using namespace Basic;

Context context("<program>");

tuple<DataPtr, shared_ptr<Error>> Basic::run(const string &filename, const string &text)
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
			cout << t.repr() << ", ";
		}
		cout << "]" << endl;
#endif
	}
	catch (Error &e)
	{
		return make_tuple(nullptr, make_shared<Error>(e));
	}

	// 当为注释时，仅有EOF
	if (lex_result.size() <= 1)
		return make_tuple(nullptr, nullptr);

	// Parsing
	Parser parse(lex_result);
	lex_result.clear();

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
		cout << root->repr() << endl;
	}
#endif

	// Interpret
	Interpreter interpreter;
	err.reset();
	RuntimeResult interprete_result = interpreter.visit(root.get(), context);
	root.reset();

	DataPtr data = interprete_result.getValuePtr();
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
	SymbolTable &global_symbol_table = context.get_symbol_table();

	// 常量定义
	global_symbol_table.set("null", make_Dataptr<Number>(Number::null));
	global_symbol_table.set("TRUE", make_Dataptr<Number>(Number::TRUE));
	global_symbol_table.set("FALSE", make_Dataptr<Number>(Number::FALSE));
	global_symbol_table.set("M_PI", make_Dataptr<Number>(Number::MATH_PI));

	// 内置函数
	global_symbol_table.set("RUN", make_Dataptr<BuiltInFunction>("RUN"));
	global_symbol_table.set("PRINT", make_Dataptr<BuiltInFunction>("PRINT"));
	global_symbol_table.set("PRINTS", make_Dataptr<BuiltInFunction>("PRINTS"));
	global_symbol_table.set("PRINT_RET", make_Dataptr<BuiltInFunction>("PRINT_RET"));

	global_symbol_table.set("INPUT", make_Dataptr<BuiltInFunction>("INPUT"));
	global_symbol_table.set("INPUT_NUM", make_Dataptr<BuiltInFunction>("INPUT_NUM"));
	global_symbol_table.set("CLEAR", make_Dataptr<BuiltInFunction>("CLEAR"));

	global_symbol_table.set("IS_NUM", make_Dataptr<BuiltInFunction>("IS_NUM"));
	global_symbol_table.set("IS_STR", make_Dataptr<BuiltInFunction>("IS_STR"));
	global_symbol_table.set("IS_LIST", make_Dataptr<BuiltInFunction>("IS_LIST"));
	global_symbol_table.set("IS_FUNC", make_Dataptr<BuiltInFunction>("IS_FUNC"));

	global_symbol_table.set("LEN", make_Dataptr<BuiltInFunction>("LEN"));
	global_symbol_table.set("APPEND", make_Dataptr<BuiltInFunction>("APPEND"));
	global_symbol_table.set("POP", make_Dataptr<BuiltInFunction>("POP"));
	global_symbol_table.set("POP_BACK", make_Dataptr<BuiltInFunction>("POP_BACK"));
	global_symbol_table.set("POP_FRONT", make_Dataptr<BuiltInFunction>("POP_FRONT"));
	global_symbol_table.set("EXTEND", make_Dataptr<BuiltInFunction>("EXTEND"));
	global_symbol_table.set("SWAP", make_Dataptr<BuiltInFunction>("SWAP"));

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
				cout << (*std::get<0>(result))->repr() << "\n";
			}
		}
		cout << endl;
	}

	crossline_history_save("history.txt");
	return 0;
}