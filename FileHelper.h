//
// Created by lianyi on 18-3-10.
//

#ifndef MYSPIDER_FILEHELPER_H
#define MYSPIDER_FILEHELPER_H

#include<string.h>
#include<unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include<string>
#include"UrlParse.h"
#include"Log.h"
#include<fcntl.h>
class FileHelper {
public:
     int CreateDir(const char *sPathName);
     int getDirAndFileName(struct Url &t,std::string &dirPath,std::string &filePath);

};


#endif //MYSPIDER_FILEHELPER_H
