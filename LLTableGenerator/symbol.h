#pragma once

#include<string>
#include<map>
#include<memory>
#include<vector>
#include<set>
#include<iostream>

using namespace std;


ostream& operator<<(ostream& os, const set<string>& set);
ostream& operator<<(ostream& os, const vector<string>& set);
set<string> operator+(const set<string>& a, const set<string>& b);

/**
 * @brief Base class for Terminals/NonTerminals
*/
class Symbol : public enable_shared_from_this<Symbol>
{
protected:
	/**
	 * @brief name of the symbol
	*/
	string name;
	/**
	 * @brief first and follow sets (LL table)
	*/
	set<string> first, follow;
	/**
	 * @brief empty identifier (LL table)
	*/
	bool empty;

	/**
	 * @brief prints symbol data based on flags
	 * @param flags flags for printing different components
	*/
	virtual void Print(unsigned int flags) const = 0;

	/**
	 * @brief Map containing all symbols
	*/
	static map<string, shared_ptr<Symbol>> table;
	
	/**
	 * @brief Pure virtual function for adding derived classes to their own tables
	*/
	virtual inline void AddToTable() = 0;

	/**
	 * @brief Utility function for using shared_from_this() in derived classes
	 * @tparam Derived Type of derived class
	 * @return shared pointer to calling derived class
	*/
	template <typename Derived>
	std::shared_ptr<Derived> shared_from_base()
	{
		return std::static_pointer_cast<Derived>(shared_from_this());
	}

public:
	/**
	 * @brief Base constructor, should not be used
	 * @param name 
	*/
	Symbol(string name);
	/**
	 * @brief Base destructor
	*/
	virtual ~Symbol();

	/**
	 * @brief Name access function
	 * @return name of the symbol
	*/
	inline string GetName() const { return name; }

	/**
	 * @brief First set access function
	 * @return First set of the symbol
	*/
	inline set<string> GetFirst() const { return first; }
	/**
	 * @brief Adds new elements to first set
	 * @param add_first elements to be added
	 * @return true if new element was added
	*/
	bool AddFirst(set<string> add_first);

	/**
	 * @brief Follow set access function
	 * @return Follow set of the symbol
	*/
	inline set<string> GetFollow() const { return follow; }
	/**
	 * @brief Adds new elements to follow set
	 * @param add_follow elements to be added
	 * @return true if new element was added
	*/
	bool AddFollow(set<string> add_follow);

	/**
	 * @brief empty access function
	 * @return true if symbol can be derived to epsilon
	*/
	inline bool IsEmpty() const { return empty; }
	/**
	 * @brief Sets empty of the symbol
	 * @param b Value to be set
	*/
	inline void SetEmpty(bool b) { empty = b; }

	/**
	 * @brief Checks if symbol is terminal
	 * @return true if terminal
	*/
	virtual inline bool IsTerminal() const = 0;

	/**
	 * @brief Inits table
	 * @param symbols Initial symbols
	 * @return Created table
	*/
	static map<string, shared_ptr<Symbol>> InitTable(map<string, shared_ptr<Symbol>> symbols);
	/**
	 * @brief Adds new symbol to the table
	 * @param symbol Symbol to be added
	 * @return Pointer to added item
	*/
	static shared_ptr<Symbol> AddSymbol(pair<string, shared_ptr<Symbol>> symbol);

	/**
	 * @brief Table access function
	 * @return Table of symbols
	*/
	static inline map<string, shared_ptr<Symbol>> Table() { return table; }
	/**
	 * @brief Prints while table based on flags
	 * @param flags flags for printing different components
	*/
	static void PrintTable(unsigned int flags);

	/**
	 * @brief Checks if whole string of symbols if empty
	 * @param vec string of symbols
	 * @return true if all symbols are empty
	*/
	static bool Empty(vector<string> vec);
	/**
	 * @brief Return all symbols that can be derived as first from string of symbols
	 * @param vec string of symbols
	 * @return derived symbols
	*/
	static set<string> First(vector<string> vec);

	/**
	 * @brief flags for printing
	*/
	enum PRINT_FLAGS
	{
		SORTED = 1 << 0, RULES = 1 << 1, FIRST = 1 << 2, EMPTY = 1 << 3, FOLLOW = 1 << 4
	};
};

