#include "../../include/Interpreter/SymbolTable.h"

namespace Basic {

	SymbolTable::SymbolTable(shared_ptr<SymbolTable> parent)
	{
		symbols.clear();
		this->parent = parent;
	}

	SymbolTable::SymbolTable(const SymbolTable& other)
	{
		this->symbols = other.symbols;
		this->parent = other.parent;
	}

	shared_ptr<Data> SymbolTable::get(string name)
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

	void SymbolTable::remove(string symbol)
	{
		symbols.erase(symbol);
	}

	void SymbolTable::set(string symbol, shared_ptr<Data> value)
	{
		symbols[symbol] = value;
	}

	void SymbolTable::setParent(shared_ptr<SymbolTable> parent)
	{
		this->parent = parent;
	}

	const map<string, shared_ptr<Data>>& SymbolTable::get_symbols()
	{
		return this->symbols;
	}

}