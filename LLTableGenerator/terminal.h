#pragma once

#include"symbol.h"

/**
 * @brief Class for Terminal symbols
*/
class Terminal : public Symbol
{
protected:
	/**
	 * @brief Prints terminal
	 * @param flags flags for printing different components
	*/
	virtual void Print(unsigned int flags) const;

	/**
	 * @brief table containing only terminal symbols
	*/
	static map<string, shared_ptr<Terminal>> table_terminal;

	/**
	 * @brief Adds new terminal to table_terminal
	*/
	virtual inline void AddToTable() { table_terminal.insert({ name, shared_from_base<Terminal>() }); }
public:
	/**
	 * @brief base constructor
	 * @param name 
	 * @param empty should be true only for epsilon
	*/
	Terminal(string name, bool empty = false);
	/**
	 * @brief base destructor
	*/
	virtual ~Terminal();

	/**
	 * @brief Overload of pure virtual function 
	 * @return true
	*/
	virtual inline bool IsTerminal() const { return true; }

	/**
	 * @brief table_terminal access function
	 * @return Table of terminals
	*/
	static inline map<string, shared_ptr<Terminal>> Table() { return table_terminal; }

	/**
	 * @brief Constructs pair element 
	 * @param name 
	 * @param empty should be true only for epsilon
	 * @return created pair
	*/
	static inline pair<string, shared_ptr<Symbol>> Pair(string name, bool empty = false) { return { name, make_shared<Terminal>(name, empty) }; }

	/**
	 * @brief friend function to allow access
	 * @param flags 
	*/
	friend static void Symbol::PrintTable(unsigned int flags);
};