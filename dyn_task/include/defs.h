#pragma once

#include <vector>
#include <tuple>
#include <utility>
#include <stack>

//timetable
// < number of task; state; start_time > 
// state = { start , continue }
typedef std::vector < std::tuple < int, int, int > > Shedule;


// < number of task, state >
typedef std::stack < std::pair < int, int > > Interrupt_tasks;

class Task;
typedef std::vector < Task * > Tasks;