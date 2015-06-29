#include "algorithm.h"

#include <tuple>
#include <utility>
#include <stdio.h>

using std::tie;
using std::tuple;
using std::make_tuple;
using std::make_pair;
using std::pair;
using std::get;

Algorithm::Algorithm ( TaskFactory * tf ) :
	taskfactory ( tf )
{}

Algorithm::~Algorithm() {
	delete taskfactory;
}

Shedule Algorithm::shedule() {
	Shedule result;//result timetable
	Interrupt_tasks interrupt_tasks;
	result.clear();
	
	for ( int curr_time = 0; curr_time < taskfactory -> runtime; curr_time++ ) {
		
		//Active task exist
		if ( !result.empty() &&  taskfactory -> isActive ( get<0>( result.back() ) ) ) {
			int ready_task = taskfactory -> readyTask( curr_time );
			//printf("Ready_task ->%d\n;curr_time ->%d", ready_task, curr_time );
			//taskfactory -> print ( ready_task );
			
			if ( ready_task >= 0 ) { //if exist ready task
				int curr_work = get<0> ( result.back() );
				//Compare priorities curr_work and ready_task
				if ( taskfactory -> tasks[ curr_work ] -> priority >= taskfactory -> tasks[ ready_task ] -> priority ) {
					taskfactory -> updateTime ( curr_work, curr_time);
					//printf("Ready_task ->%d\n;curr_time ->%d", ready_task, curr_time );
					//printf("Ready_task ->%d\n;curr_time ->%d", curr_work, curr_time );
					//exit(0);
					continue;
				}
				else { //if curr_work.priority < ready_task.priority therefore interrupt current work
					//state = 0 when work not interrupt. If work interrupted ( <continue> ) state = 1
					interrupt_tasks.push( make_pair ( get<0>( result.back() ), 1 ) );
					result.push_back ( make_tuple ( ready_task, 0, curr_time ) );
					taskfactory -> startWork( ready_task );
					taskfactory -> updateTime ( ready_task, curr_time);
				}
			} 
			else { // interrupt tasks have priority < than priority of active tasks
				taskfactory -> updateTime ( get<0> ( result.back() ), curr_time);
				continue;
			}
		}
		else { //if there is not active tasks
			int ready_task = taskfactory -> readyTask( curr_time );
			//printf("Ready_task ->%d\n;curr_time ->%d", ready_task, curr_time );
			//taskfactory -> print ( ready_task );
			
			
			if ( ready_task >= 0 ) { //if exist ready task
				if ( interrupt_tasks.empty() ) { //put in execution this ready task
					result.push_back ( make_tuple ( ready_task, 0, curr_time ) );
					taskfactory -> startWork( ready_task );
					taskfactory -> updateTime ( ready_task, curr_time);
				}
				else { //if interrupt tasks exist
					int inter_task = interrupt_tasks.top().first;
					if ( taskfactory -> tasks[ ready_task ] -> priority > taskfactory -> tasks[ inter_task ] -> priority ) {
						result.push_back ( make_tuple ( ready_task, 0, curr_time ) );
						taskfactory -> startWork( ready_task );
						taskfactory -> updateTime ( ready_task, curr_time);
					}
					else { //inter task priority >= ready task priority therefore put in execution interrupt work
						interrupt_tasks.pop();//delete interrupted work from stack
						result.push_back ( make_tuple ( inter_task, 1, curr_time ) );
						//taskfactory -> startWork( inter_task );//Не нужно опять сбрасывать время time_to_end до duration
						//работа уже выполнялась => просто продолжаем
						taskfactory -> updateTime ( inter_task, curr_time);
					}
				}
			}
			else { //There are no ready tasks or active tasks
				if ( interrupt_tasks.empty() ) continue;
				else { //if interrupt tasks exist
					int inter_task = interrupt_tasks.top().first;
					interrupt_tasks.pop();
					result.push_back ( make_tuple ( inter_task, 1, curr_time ) );
					//так как данная работа уже выполнялась, следовательно,
					//time_to_end уже меньше duration, но больше нуля
					//нужно просто возобновить обратный отсчет
					//taskfactory -> startWork( inter_task );//Не нужно опять сбрасывать время time_to_end до duration
					//работа уже выполнялась => просто продолжаем
					
					taskfactory -> updateTime ( inter_task, curr_time);
				}
			}

		}
	}
	
	return result;
}