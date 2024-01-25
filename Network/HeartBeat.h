#pragma once
#include "../stdafx.h"






class Work;
class SessionMonitor 
{
public :
	SessionMonitor();
	void Tick();
};
 
static SessionMonitor sessionMonitor;