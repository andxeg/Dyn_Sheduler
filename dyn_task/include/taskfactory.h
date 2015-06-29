#pragma once

#include "defs.h"
#include "task.h"

#include <vector>
#include <stdlib.h>
#include <string.h>
#include <iostream>

class TaskFactory {
	friend class Algorithm;
	friend class Snapshot;
public:
	TaskFactory ( const Tasks & t ) :
		tasks( t )
	{}
	
	~TaskFactory() { //delete all tasks
		for ( uint i = 0  ; i < tasks.size(); i++ )
			delete tasks[i];
	}
	
	int readyTask ( const int & curr_time ) { //return number of ready task with the highest priority
		std::vector< uint > ready_tasks;
		for ( uint i = 0; i < tasks.size(); i++ ) {
			//( ( time_last_end div period ) + 1 ) * period <= curr_time
			//watch only inactive task
			if ( tasks[i] -> isActive() ) continue;
			
			if ( tasks[i] -> time_last_end == 0 && tasks[i] -> time_to_end == 0 ) {
				ready_tasks.push_back( i );
				continue;
			}
			
			if ( ( ( tasks[i]-> time_last_end / tasks[i]->period ) + 1 ) * tasks[i]->period <= curr_time )
				ready_tasks.push_back( i );
		}
		if ( ready_tasks.size() == 0 ) return -1;
		
		int task_number = -1;//number of task with the highest priority
		int highest_priority = -1;
		for ( uint i = 0; i < ready_tasks.size(); i++ )
			if ( tasks[ ready_tasks [ i ] ] -> priority > highest_priority )
				highest_priority = tasks[ ready_tasks [ i ] ] -> priority;
		
		std::vector< uint > ready_task_with_equal_priority;
		for ( uint i = 0; i < ready_tasks.size(); i++ )
			if ( tasks[ ready_tasks [ i ] ] -> priority == highest_priority  )
				ready_task_with_equal_priority.push_back ( ready_tasks [ i ] );
		
		//Choose one random task from task pool with equal priority
		task_number = ready_task_with_equal_priority[ rand() % ready_task_with_equal_priority.size() ];
		
		return task_number;
	}
	
	void updateTime ( const int & number, const int & curr_time ) { //update field "time_to_end" in task --> tasks [ number ]
		if ( tasks [ number ] -> time_to_end == 1  )
			tasks [ number ] -> time_last_end = curr_time + 1;
		
		if ( tasks [ number ] -> time_to_end != 0 )
			tasks [ number ] -> time_to_end -= 1; 
	}
	
	void startWork ( const int & number ) {
		tasks[ number ] -> time_to_end = tasks[ number ] -> duration;
	}
	
	int getRuntime () {
		return runtime;
	}
	
	void setRuntime ( const int & r_time) {
		runtime = r_time;
	}
	
	bool isActive ( const int & task_number ) {
		return tasks [ task_number ] -> isActive();
	}
	
	void print() {
		std::cout << "<system runtime=\"" << runtime << "\">" << std::endl;
		
		for ( uint i = 0; i < tasks.size(); ++i ) {
			std::string name = tasks[i] -> name;
			int period = tasks[i] -> period;
			int duration = tasks[i] -> duration;
			int priority = tasks[i] -> priority;
			std::cout << "<task name=\""<< name <<"\" period=\"" << period <<"\" priority=\"" 
			<< priority <<"\" duration=\"" << duration <<"\"/>" << std::endl;
		}
		
		std::cout << "</system>" << std::endl;
	}
	
	void print ( const int & i ) {
		std::string name = tasks[i] -> name;
		int period = tasks[i] -> period;
		int duration = tasks[i] -> duration;
		int priority = tasks[i] -> priority;
		std::cout << "<task name=\""<< name <<"\" period=\"" << period <<"\" priority=\"" 
		<< priority <<"\" duration=\"" << duration <<"\"/>" << std::endl;
	}
	
private:
	Tasks tasks;//input tasks
	int runtime;
};