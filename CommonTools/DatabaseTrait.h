#pragma once

#include <sqlite3pp/sqlite3pp.h>

template<class T>
struct Trait
{

};

template<>
struct Trait<sqlite3pp::database>
{
	typedef typename sqlite3pp::database DB;
	typedef typename sqlite3pp::command DBCmd;
	typedef typename sqlite3pp::query DBQry;
};
