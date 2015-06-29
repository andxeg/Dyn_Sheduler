#include "snapshot.h"
#include "task.h"
#include "taskfactory.h"
#include "tinyxml2.h"
#include "defs.h"

#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <utility>

using namespace tinyxml2;
using std::get;

Snapshot::Snapshot()
{
	document = new XMLDocument();
}

Snapshot::~Snapshot()
{
	delete document;
}

TaskFactory * Snapshot::read ( const std::string & filename )
{
	//Load xml file
	if (  document -> LoadFile ( filename.c_str() ) ) {
		printf("Error while reading file\n");
		return 0;
	}
	//
	
	//Check root element
	XMLElement * root = document -> RootElement();
	
	if ( strcmp( root -> Name(),"system\0" ) != 0 ) {
		printf("Error in root name -> \n");
		return 0;
	}
	//
	
	//Check root's attributes
	const XMLAttribute * attr = root -> FirstAttribute();
	if ( attr == 0 ) {
		printf("There are not attributes in element -> system \n");
		return 0;
	}
	int runtime;
	if ( strcmp( attr -> Name(),"runtime\0" ) != 0 ) {
		printf("Error while reading first attributes in element -> system \n");
		return 0;
	}
	

	if ( attr -> QueryIntValue ( &runtime ) == XMLError::XML_WRONG_ATTRIBUTE_TYPE ) {
		printf("Error while reading value of attribute -> runtime\n");
		return 0;
	}
	
	if ( runtime <= 0 ) {
		printf("Error in runtime\n");
		return 0;
	}
	
	if ( attr -> Next() != 0 ) {
		printf("Extra attributes in element -> system\n");
		return 0;
	}
	//
	
	//Check existence of tasks
	if ( root -> NoChildren() ) {
		printf("Element system hasn't children\n");
		return 0;
	}
	//
	//one task exist exactly
	const XMLNode * taskXML = root -> FirstChild();
	
	Tasks inputTasks;
	
	while ( taskXML != 0 ) {
		//Check tag name 
		if ( strcmp( taskXML->ToElement()-> Name(),"task\0" ) != 0 ) {
			printf("Error in child element -> %s \n", taskXML->ToElement()-> Name() );
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
		//
		
		//Check children
		if ( !taskXML -> ToElement()-> NoChildren() ) { 
			printf ("Error: %s have children", taskXML->ToElement()-> Name() );
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
		//
		
		//READ ATTRIBUTES
		std::string name;
		int period;
		int priority;
		int duration;
		
		const XMLAttribute * attrib = taskXML -> ToElement() -> FirstAttribute();
		
			//============Read name of task==============
		if ( strcmp( attrib -> Name(),"name\0" ) != 0 ) {
			printf("Error while reading first attribute in element -> task \n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
		name = std::string ( attrib -> Value() );
			//===========================================
		
			//================Read period================
		if ( attrib -> Next() == 0 ) {
			printf("Period is not state \n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
		
		attrib = attrib -> Next();
		
		if ( strcmp( attrib -> Name(),"period\0" ) != 0 ) {
			printf("Error while reading second attribute in element -> task \n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
		
		if ( attrib -> QueryIntValue ( &period ) == XMLError::XML_WRONG_ATTRIBUTE_TYPE ) {
			printf("Error while reading period\n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
		
		if ( period <= 0 ) {
			printf("Error in period\n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
			//===========================================
		
			//================Read priority==============
		if ( attrib -> Next() == 0 ) {
			printf("Priority is not state \n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}

		attrib = attrib -> Next();

		if ( strcmp( attrib -> Name(),"priority\0" ) != 0 ) {
			printf("Error while reading third attribute in element -> task \n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}

		if ( attrib -> QueryIntValue ( &priority ) == XMLError::XML_WRONG_ATTRIBUTE_TYPE ) {
			printf("Error while reading priority\n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
		
		if ( priority <= 0 ) {
			printf("Error in priority\n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
			//===========================================
		
			//================Read duration==============
		if ( attrib -> Next() == 0 ) {
			printf("Duration is not state \n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}

		attrib = attrib -> Next();

		if ( strcmp( attrib -> Name(),"duration\0" ) != 0 ) {
			printf("Error while reading forth attribute in element -> task \n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}

		if ( attrib -> QueryIntValue ( &duration ) == XMLError::XML_WRONG_ATTRIBUTE_TYPE ) {
			printf("Error while reading duration\n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
		
		if ( duration <= 0 ) {
			printf("Error in duration\n");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
			//===========================================
		
		//Duration should be less than period
		if ( duration > period ) {
			printf("Error: duration should be less than period");
			Snapshot::deleteTasks( inputTasks );
			return 0;
		}
		//
		
		//
		
		//Create Task
		Task * curr_task = new Task();
		curr_task -> name = name;
		curr_task -> period =period;
		curr_task -> priority = priority;
		curr_task -> duration = duration;
		curr_task -> time_to_end = 0;
		curr_task -> time_last_end = 0;
		inputTasks.push_back( curr_task );
		//
		
		//Next node
		taskXML = taskXML -> NextSibling();
		//
	}
	
	TaskFactory * factory = new TaskFactory ( inputTasks );
	factory -> setRuntime( runtime );
	//print ( factory );
	return factory;
}

void Snapshot::deleteTasks( const Tasks & tasks ) {
	for ( uint i = tasks.size() ; i >= 0; i-- )
			delete tasks[i];
}

bool Snapshot::write ( const std::string & filename, Shedule result, TaskFactory * taskfactory )
{
	std::fstream output ( filename, std::fstream::out);
	
	output << "<trace runtime=\"" << taskfactory -> runtime << "\">" << std::endl;
	
	for ( uint i = 0; i < result.size(); i++ ) {
		std::string name = taskfactory -> tasks [ get<0>( result[i] ) ] -> name;
		int start = get<2> ( result[i] );
		if ( get<1>( result[i] ) == 0 ) output << "<start ";
		else output << "<continue ";
		output << "name=\""<< name <<"\" time=\""<< start << "\"/>" << std::endl; 
	}
	
	output << "</trace>" << std::endl;
	output.close();
	return true;
}

void Snapshot::print( TaskFactory * taskfactory ) {
	std::cout << "<system runtime=\"" << taskfactory -> runtime << "\">" << std::endl;
	
	for ( uint i = 0; i < taskfactory -> tasks.size(); ++i ) {
		std::string name = taskfactory -> tasks[i] -> name;
		int period =taskfactory -> tasks[i] -> period;
		int duration = taskfactory -> tasks[i] -> duration;
		int priority = taskfactory -> tasks[i] -> priority;
		std::cout << "<task name=\""<< name <<"\" period=\"" << period <<"\" priority=\"" 
		<< priority <<"\" duration=\"" << duration <<"\"/>" << std::endl;
	}
	
	std::cout << "</system>" << std::endl;
}