#pragma once

#include <map>
#include <string>
#include <memory>

using std::map;
using std::string;
using std::shared_ptr;
using std::make_shared;

namespace Basic {

	class Data;

	// 该类用于存储某一个阶段（函数）产生的变量，以及其值
	class SymbolTable
	{
	public:
		SymbolTable(shared_ptr<SymbolTable>parent = nullptr);
		SymbolTable(const SymbolTable&);

		shared_ptr<Data> get(string name);
		void remove(string symbol);
		void set(string symbol, shared_ptr<Data> value);
		void setParent(shared_ptr<SymbolTable>);

		const map<string, shared_ptr<Data>>& get_symbols();

	private:
		map<string, shared_ptr<Data>> symbols;	// 当一个函数结束时，释放临时变量
		shared_ptr<SymbolTable> parent;	// 相对与当前函数而言的“全局变量”，对于主函数而言，为空
	};

}