#include "../../include/Interpreter/SymbolTable.h"
#include "../../include/Interpreter/Data.h"

namespace Basic
{
	SymbolTable::SymbolTable(const shared_ptr<SymbolTable> &parent)
	{
		symbols.clear();
		this->parent = parent;
	}

	SymbolTable::SymbolTable(const SymbolTable &other)
	{
		*this = other; // 调用赋值运算符
	}

	SymbolTable &SymbolTable::operator=(const SymbolTable &other)
	{
		this->symbols = other.symbols;
		this->parent = other.parent;
		return *this;
	}

	shared_ptr<unique_ptr<Data>> SymbolTable::get(const string &name)
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

	void SymbolTable::remove(const string &symbol)
	{
		symbols.erase(symbol);
	}

	void SymbolTable::set(const string &symbol, const shared_ptr<unique_ptr<Data>> &value)
	{
		symbols[symbol] = value;
	}

	void SymbolTable::setParent(const shared_ptr<SymbolTable> &parent)
	{
		this->parent = parent;
	}

	const map<string, shared_ptr<unique_ptr<Data>>> &SymbolTable::get_symbols()
	{
		return this->symbols;
	}
}