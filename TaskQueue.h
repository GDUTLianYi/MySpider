//
// Created by lianyi on 18-2-25.
//

#ifndef MYSPIDER_TASKQUENE_H
#define MYSPIDER_TASKQUENE_H

#include<string>
#include<queue>
#include <map>
#include "UrlParse.h"
#include "Log.h"

class TaskQueue {

public:
    int Run();
    int init(std::string url);

private:
    UrlParse m_UrlParse;
};


#endif //MYSPIDER_TASKQUENE_H
