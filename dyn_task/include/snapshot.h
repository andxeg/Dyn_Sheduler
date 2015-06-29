#pragma once

#include "defs.h"
#include "taskfactory.h"
#include "tinyxml2.h"

#include <string>


class Snapshot {
public:
	Snapshot();
	~Snapshot();
	TaskFactory * read ( const std::string & filename );
	bool write ( const std::string & filename, Shedule result, TaskFactory * factory );
	void deleteTasks ( const Tasks & tasks );
	void print( TaskFactory * factory );
private:
	tinyxml2::XMLDocument * document;
};