#pragma once

#include <string.h>


class Task {
	friend class TaskFactory;
	friend class Snapshot;
	friend class Algorithm;
public:
	bool isActive() {
		return time_to_end == 0 ? false : true;
	}
private:
	std::string name;
	int period;
	int priority;
	int duration;
	/*when work put in execution, time_to_end = duration;
	 * if ( time_to_end == 0 ) task is inactive
	 */
	int time_to_end;
	int time_last_end;//end of the last execution interval
};
