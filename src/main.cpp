#include "Common/utils.h"
#include "Common/argparse.h"
#include "Common/Context.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Interpreter/Interpreter.h"

using namespace std;
using namespace Basic;

Context context("<program>");
bool DEBUG = false;

tuple<DataPtr, shared_ptr<Error>> Basic::run(const string &filename, const string &text)
{
	// LexicalAnalysis
	Lexer lexer(filename, text);
	vector<Token> lex_result;
	try
	{
		lex_result = lexer.make_tokens();
		if (DEBUG)
		{
			cout << "\n[";
			for (Token &t : lex_result)
			{
				cout << t.repr() << ", ";
			}
			cout << "]" << endl;
		}
	}
	catch (IllegalCharError &e)
	{
		return make_tuple(nullptr, make_shared<IllegalCharError>(e));
	}
	catch (ExpectCharError &e)
	{
		return make_tuple(nullptr, make_shared<ExpectCharError>(e));
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

	if (DEBUG)
	{
		cout << root->repr() << endl;
	}

	// Interpret
	Interpreter interpreter;
	RuntimeResult interprete_result = interpreter.visit(root, context);

	DataPtr data = interprete_result.getValuePtr();
	err = interprete_result.getError();

	if (err != nullptr)
	{
		return make_tuple(nullptr, err);
	}
	else if (data != nullptr && typeid(**data) != typeid(Data))
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
}

void doREPL()
{
	cout << "Initialization completed, welcome to Basic\n\n";

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
}

struct MyArgs : public argparse::Args
{
	std::optional<string> &src_path = kwarg("f,file", "Execute Basic script from given file_path");
	std::optional<string> &text = kwarg("t,text", "Execute Basic script from given text");
	bool &interactive = flag("i", "A flag to toggle interactive mode");
	bool &verbose = flag("v,verbose", "A flag to toggle verbose");
	bool &debug = flag("D,Debug", "A flag to toggle debug mode");

	void welcome() override
	{
		cout << "Welcome to Basic!\n";
	}
};

void ParseArgs(int argc, char *argv[])
{
	MyArgs args = argparse::parse<MyArgs>(argc, argv);

	if (args.verbose)
		args.print();

	if (args.debug)
		DEBUG = true;

	if (args.src_path.has_value())
	{
		string file = args.src_path.value();
		auto text = readfile(file);
		if (text.has_value())
		{
			auto result = run(file, text.value());
			if (std::get<1>(result) != nullptr)
			{
				cout << std::get<1>(result)->as_string() << "\n";
			}
		}
		else
		{
			cout << "Fatal: cannot load script\n";
		}
	}

	if (args.text.has_value())
	{
		auto result = run("<stdin>", args.text.value());
		if (std::get<1>(result) != nullptr)
		{
			cout << std::get<1>(result)->as_string() << "\n";
		}
	}

	if (args.interactive)
		doREPL();
}

int main(int argc, char *argv[])
{
	Init();

	if (argc == 1)
		doREPL();
	else
		ParseArgs(argc, argv);

	return 0;
}