//
// Created by lianyi on 18-3-10.
//

#include "FileHelper.h"
 int FileHelper::CreateDir(const char *sPathName) {
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

 int FileHelper::getDirAndFileName(struct Url &t,std::string &dirPath,std::string &filePath){
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