//
// Created by lianyi on 18-3-10.
//

#ifndef MYSPIDER_TASKQUEUESELECT_H
#define MYSPIDER_TASKQUEUESELECT_H
#include<string>
#include<queue>
#include <map>
#include "UrlParse.h"
#include "Log.h"

class TaskQueueSelect {

public:
    int Run();
    int init(std::string url);

private:
    UrlParse m_UrlParse;
};


#endif //MYSPIDER_TASKQUEUESELECT_H
