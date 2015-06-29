#pragma once

#include "defs.h"
#include "taskfactory.h"

class Algorithm {
public:
	Algorithm ( TaskFactory * tf );
	~Algorithm();//delete taskfactory
	Shedule shedule();//create a shedule
private:
	TaskFactory * taskfactory; 
};