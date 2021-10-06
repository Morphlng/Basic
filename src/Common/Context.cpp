#include "Common/Context.h"

namespace Basic
{
	Context::Context(const string &display_name, const shared_ptr<Context> &parent, const Position &parent_entry_pos)
	{
		this->display_name = display_name;
		this->parent = parent;
		this->parent_entry_pos = parent_entry_pos;
	}

	Context::Context(const Context &other)
	{
		this->display_name = other.display_name;
		this->parent_entry_pos = other.parent_entry_pos;
		this->parent = other.parent;
		this->symbol_table = other.symbol_table;
	}

	void Context::set_symbol_table(const SymbolTable &symble_table)
	{
		this->symbol_table = symble_table;
	}

	const string &Context::get_displayName()
	{
		return this->display_name;
	}

	const Position &Context::get_parent_entry_pos()
	{
		return this->parent_entry_pos;
	}

	const shared_ptr<Context> &Context::get_parent()
	{
		return this->parent;
	}

	SymbolTable &Context::get_symbol_table()
	{
		return this->symbol_table;
	}
}