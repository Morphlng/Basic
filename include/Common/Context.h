#pragma once

#include <memory>
#include <string>
#include "Position.h"
#include "../Interpreter/SymbolTable.h"

using std::shared_ptr;
using std::string;

namespace Basic {

	// 该类用于记录当前程序运行的“上下文”，可用于错误栈定位
	class Context
	{
	public:
		Context(const string& display_name = "", const shared_ptr<Context>& parent = nullptr, const Position& parent_entry_pos = Position());
		Context(const Context&);
		void set_symbol_table(const SymbolTable&);

		const string& get_displayName();
		const Position& get_parent_entry_pos();
		const shared_ptr<Context>& get_parent();
		SymbolTable& get_symbol_table();

	private:
		string display_name;	   // 当前环境名称
		Position parent_entry_pos; // “上文”的错误定位
		shared_ptr<Context> parent;		   // 指向“上文”
		SymbolTable symbol_table;  // 当前环境中的变量集合
	};
}