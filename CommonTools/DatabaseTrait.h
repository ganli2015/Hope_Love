#pragma once

//#include <sqlite3pp/sqlite3pp.h>

namespace sqlite3pp
{
	class database;
	class command;
	class query;
}

template<class T>
struct Trait
{
	typedef typename T::database DB;
	typedef typename T::command DBCmd;
	typedef typename T::query DBQry;
};

template<>
struct Trait<sqlite3pp::database>
{
	typedef typename sqlite3pp::database DB;
	typedef typename sqlite3pp::command DBCmd;
	typedef typename sqlite3pp::query DBQry;
};
