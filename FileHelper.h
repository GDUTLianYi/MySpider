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
    static int CreateDir(const char *sPathName) {
        char DirName[256];
        strcpy(DirName, sPathName);
        int i, len = strlen(DirName);
        if (DirName[len - 1] != '/')
            strcat(DirName, "/");

        len = strlen(DirName);

        for (i = 1; i < len; i++) {
            if (DirName[i] == '/') {
                DirName[i] = 0;
                if (access(DirName, NULL) != 0) {
                    if (mkdir(DirName, 0755) == -1) {
                        perror("mkdir error");
                        return -1;
                    }
                }
                DirName[i] = '/';
            }
        }
        return 0;
    }

    static int getDirAndFileName(struct Url &t,std::string &dirPath,std::string &filePath){
        std::string DownLoadPath = "/home/lianyi/MySpideDownLoad";
        std::string::size_type pos = t.path.find_last_of('/');


        std::string SavePath = DownLoadPath + t.path;
        std::string dir = DownLoadPath;
        std::string FileName("");
        if (pos != std::string::npos) {
            dir += t.path.substr(0, pos + 1);
            FileName = t.path.substr(pos + 1);
        }
        if (t.path.empty() || t.path == "/" || FileName.empty()) {
            if (SavePath.back() != '/') {
                SavePath.push_back('/');
            }
            SavePath += t.domain + ".html";
        } else {

            if (FileName.find('.') == std::string::npos) {
                SavePath += ".html";
            }
        }
        filePath=SavePath;
        dirPath=dir;
        return 0;
    }

//    static int GetFileFd(){
//        std::string DirPath, FilePath;
//        FileHelper::getDirAndFileName(file[i].t, DirPath, FilePath);
//        FileHelper::CreateDir(DirPath.c_str());
//        int Fd_SaveFile = open(FilePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, MODE);
//        if (Fd_SaveFile == -1) {
//            close(tmpfd);
//            Log::unix_error("create open file error");
//            continue;
//        }
//    }
};


#endif //MYSPIDER_FILEHELPER_H
