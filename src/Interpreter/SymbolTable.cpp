#include "../../include/Interpreter/SymbolTable.h"

namespace Basic {

	SymbolTable::SymbolTable(const shared_ptr<SymbolTable>& parent)
	{
		symbols.clear();
		this->parent = parent;
	}

	SymbolTable::SymbolTable(const SymbolTable& other)
	{
		this->symbols = other.symbols;
		this->parent = other.parent;
	}

	shared_ptr<Data> SymbolTable::get(const string& name)
	{
		auto result = symbols.find(name);
		if (result != symbols.end())
			return result->second;
		else if (parent != nullptr)
		{
			return (*parent).get(name);
		}

		return nullptr;
	}

	void SymbolTable::remove(const string& symbol)
	{
		symbols.erase(symbol);
	}

	void SymbolTable::set(const string& symbol, const shared_ptr<Data>& value)
	{
		symbols[symbol] = value;
	}

	void SymbolTable::setParent(const shared_ptr<SymbolTable>& parent)
	{
		this->parent = parent;
	}

	const map<string, shared_ptr<Data>>& SymbolTable::get_symbols()
	{
		return this->symbols;
	}

}