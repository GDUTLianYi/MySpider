//
// Created by lianyi on 18-2-27.
//

#include "UrlParse.h"
#include "FileHelper.h"

#define MODE S_IRUSR|S_IWUSR|S_IXUSR

int threadNum=0;
extern int FinshSign;
pthread_mutex_t mutex_ipMap = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Que_RegexFile = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Set_Url = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_threadNum = PTHREAD_MUTEX_INITIALIZER;
typedef std::unordered_set<struct Url, UrlKey, UrlCmp> urlSet;
std::unordered_map<std::size_t, urlSet> ipMap;
std::queue<RegexPackage> Que_RegexFile;
std::queue<Url> BeforeUrl;
std::unordered_set<std::size_t> Set_Url;

std::tuple<std::string, std::string, std::string> ToParseUrl(std::string url);


std::size_t GetHashcode(std::string s) {
    return std::hash<std::string>{}(s);
}

void *SpiderThreadFunc(void *agrc);
int xxxx=1;
int AddUrl(struct Url &url){
    myDNS dnsHeler;

    std::size_t hashurl = GetHashcode(url.url);
    do {
        std::cout<<"apply mutex_Set_Url"<<std::endl;
        MutexRAII<pthread_mutex_t> lcks(mutex_Set_Url);
        std::cout<<"apply mutex_Set_Url OK "<<std::endl;
        if (Set_Url.count(hashurl)) {
            return 1;
        } else {
            Set_Url.insert(hashurl);
        }
    } while (0);


    char *ip = dnsHeler.GetHostByName(url.domain);
    if (ip == NULL) {
        return 2;
    }

    url.ip = ip;
    free(ip);
    std::cout<<"apply mutex_ipMap"<<std::endl;
    MutexRAII<pthread_mutex_t> lck(mutex_ipMap);
     std::cout<<"apply mutex_ipMap OK "<<std::endl;


    size_t domainHash = GetHashcode(url.domain);

//    do{
//        MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
//        if(threadNum<40){
//            flag=1;
//        }
//    }while(0);


    if (1||ipMap.find(domainHash) == ipMap.end()) {
        std::cout<<"does two"<<std::endl;

        //  std::cout<<url.domain<<"  "<<url.ip<<std::endl;
        threadNum++;
        std::cout<<"threadNum "<<threadNum<<std::endl;
        pthread_t tid;
        int terror;
        //  std::cout<<"create thread "<<tid <<" domainhash="<<domainHash <<std::endl;
        terror = pthread_create(&tid, NULL, SpiderThreadFunc, new size_t(domainHash));

        // terror= pthread_create(&tid,NULL,SpiderThreadFunc,NULL);
        // std::cout<<"create thread "<<tid <<" domainhash="<<domainHash <<std::endl;
        if (terror != 0) {
            Log::unix_error("error (pthread_create)  ");
            return 3;
        }else{

        }
    }

    ipMap[domainHash].insert(url);
   // std::cout<<"add Url  :domain= "<<url.domain <<"  "<<url.url<<std::endl;
    return 0;
}

void GetUrl(const char *buf, int len, std::string domain, int port) {

    std::string str = std::string(buf, buf + len);
    boost::regex reg_url(pattern_url, boost::regex_constants::icase);
    boost::sregex_iterator rit(str.begin(), str.end(), reg_url);
    boost::sregex_iterator rend;

    for (; rit != rend; rit++) {
        struct Url tmp;
        tmp.url = rit->str();
        tmp.domain = rit->str(2);
        if (!rit->str(3).empty())
            tmp.port = stoi(rit->str(3));
        else
            tmp.port = 80;
        tmp.path = rit->str(4);
        tmp.dep = 0;
        tmp.state = 0;
        AddUrl(tmp);
    }
    boost::regex reg_href(pattern_href, boost::regex_constants::icase);
    boost::sregex_iterator rit_href(str.begin(), str.end(), reg_href);
    for (; rit_href != rend; rit_href++) {
        struct Url tmp;
        tmp.url = "http://" + domain + rit_href->str(2);
        tmp.domain = domain;
        tmp.port = port;
        tmp.path = rit_href->str(2);
        tmp.dep = 0;
        tmp.state = 0;
        AddUrl(tmp);
    }
}


std::tuple<std::string, std::string, std::string> ToParseUrl(std::string url) {

    boost::regex reg(pattern_url);
    std::string domain(""), port(""), path("");
    boost::smatch sm;
    if (boost::regex_match(url, sm, reg)) {
        domain = sm[2];
        port = sm[3];
        path = sm[4];
    }
    return std::make_tuple(domain, port, path);
}


void *SpiderThreadFunc(void *agrc) {


    int timeout = 0;
    size_t ac = *((std::size_t *) agrc);
    free(agrc);
    size_t hashdomain = ac;
    std::cout << "in thread  " << hashdomain << std::endl;
    std::string ip;
    int port;
    do {


        std::cout<<"apply mutex_ipMap"<<std::endl;
        MutexRAII<pthread_mutex_t> lock(mutex_ipMap);
        std::cout<<"apply mutex_ipMap OK "<<std::endl;
        if (ipMap.find(hashdomain) == ipMap.end()) {

            std::cout << "what " << std::endl;
//            MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);

            std::cout<<"threadNum "<<threadNum--<<std::endl;
            pthread_exit(NULL);
        }

        ip = (ipMap[hashdomain].begin())->ip;
        port = (ipMap[hashdomain].begin())->port;

    } while (0);
    char sdbuf[9192];
    CSocket socketFactory;
    Http httpHelper;
    // std::cout<<"create socket"<<std::endl;
    int clientfd = socketFactory.open_clientfd(ip.c_str(), port);
    //std::cout<<"create socket ok  "<<clientfd <<std::endl;


    if (clientfd == -1) {
        MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
        std::cout<<"threadNum "<<threadNum--<<std::endl;
        pthread_exit(NULL);
    }

    Url tmpurl;
    int flag1;
   // for (; FinshSign != 1;) {

        std::cout << "[ hashdomain ]" << hashdomain << std::endl;
        //loop:;
        do {
            if (flag1 == 0) {

                if (timeout++ >3){
                    // close(clientfd);
                    // std::cout<<"CLOSE clientfd  "<< clientfd<<std::endl;
                  //  close(clientfd);
                   // MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
                    //threadNum--;
                  //  pthread_exit(NULL);
                }
                sleep(5);
            }


            MutexRAII<pthread_mutex_t> lock(mutex_ipMap);
            if (ipMap[hashdomain].empty()) {
                ipMap.erase(hashdomain);

                flag1 = 0;
              //  goto loop;
                MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
                std::cout<<"threadNum "<<threadNum--<<std::endl;
                pthread_exit(NULL);
            }
            tmpurl = *ipMap[hashdomain].begin();
            ipMap[hashdomain].erase(ipMap[hashdomain].begin());


        } while (0);



        httpHelper.Form_Get(sdbuf, tmpurl);
          std::cout<<"want to send :"<<sdbuf<<std::endl;
        if (send(clientfd, sdbuf, strlen(sdbuf), 0) == -1) {
            Log::unix_error("send get error");
//            MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
//            threadNum--;
            MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
            std::cout<<"threadNum "<<threadNum--<<std::endl;
      pthread_exit(NULL);
           // continue;
        } else {
            //std::cout<<sdbuf<<std::endl;
        }

        Rio m_rio;
        rio_t rio;
        m_rio.readinitb(&rio, clientfd);


        ssize_t rn;
        int state = -1;
        char pro[20], res[20];
        while ((rn = m_rio.readlineb(&rio, sdbuf, 8196)) > 0) {
            // std::cout << sdbuf;
            if (rn < 0) {
                close(clientfd);
                MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
                std::cout<<"threadNum "<<threadNum--<<std::endl;
                pthread_exit(NULL);
            }
            if (state == -1) {
                sscanf(sdbuf, "%s %d %s", pro, &state, res);
            }
            if (sdbuf[0] == '\r' && sdbuf[1] == '\n') {
                break;
            }
        }

        if (state != 200) {
            MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
            std::cout<<"threadNum "<<threadNum--<<std::endl;
            pthread_exit(NULL);
        }
        std::string DirPath, FilePath;
        FileHelper fileHelper;
        fileHelper.getDirAndFileName(tmpurl, DirPath, FilePath);
        fileHelper.CreateDir(DirPath.c_str());
        int Fd_SaveFile = open(FilePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, MODE);
        if (Fd_SaveFile == -1) {
            Log::unix_error("create open file error");
            MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
            std::cout<<"threadNum "<<threadNum--<<std::endl;
           pthread_exit(NULL);
        }

        //std::cout << FilePath << std::endl;
        while ((rn = m_rio.readnb(&rio, sdbuf, 8196)) > 0) {
            // std::cout<<i++<<std::endl;
            if (rn < 0) {
                close(clientfd);
                MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
                std::cout<<"threadNum "<<threadNum--<<std::endl;
//                MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
//                threadNum--;
                pthread_exit(NULL);
            }
            std::cout << " " << rn << std::endl;
            write(Fd_SaveFile, sdbuf, rn);
        }

        close(Fd_SaveFile);


        do {

            std::cout<<"apply mutex_Que_RegexFile"<<std::endl;
            MutexRAII<pthread_mutex_t> lockf(mutex_Que_RegexFile);
            std::cout<<"apply mutex_Que_RegexFile OK "<<std::endl;
            // std::cout<<"add FIlE :: "<<FilePath<<std::endl;
            Que_RegexFile.push(RegexPackage{FilePath, tmpurl.domain, tmpurl.port});
        } while (0);

   // }

//    MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
//    threadNum--;
    MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
    std::cout<<"threadNum "<<threadNum--<<std::endl;
    return NULL;
}


void *RegexThreadFunc(void *argc) {



    Rio m_rio;
    rio_t rio;
    char sdbuf[8192];
    RegexPackage rp;
    int flag;

    for (; FinshSign != 1;) {
    int xxxx=1;
        do {
            sleep(1);
            MutexRAII<pthread_mutex_t> loxx(mutex_threadNum);
            if (threadNum > 1000) {
                xxxx = 0;
            }
        }while(0);

        if(xxxx==0){
            sleep(20);
            continue;
        }
        do {
            //std::cout<<"here"<<std::endl;
            if (flag == 0) {
                sleep(5);
            }
            MutexRAII<pthread_mutex_t> lockf(mutex_Que_RegexFile);
            if (Que_RegexFile.empty()) {

                //std::cout<<"sleep"<<std::endl;
                flag = 0;
                break;
            }
            flag = 1;
            rp = Que_RegexFile.front();
            Que_RegexFile.pop();
            // std::cout<<"take out from queRgex"<<std::endl;
        } while (0);

        if (flag == 0) {
            continue;
        }
        flag = 1;
        int fd = open(rp.file.c_str(), O_RDWR, MODE);
        std::cout << "open file" << rp.file << std::endl;
        if (fd == -1) {

            Log::unix_error("create open file error");
            continue;
        }
        //   m_rio.readinitb(&rio,fd);
        ssize_t rn;
//        while ((rn = m_rio.readnb(&rio, sdbuf, RIO_BUFSIZE)) > 0) {
//            // std::cout<<i++<<std::endl;
//              std::cout<<"Regex"<<std::endl;
//            GetUrl(sdbuf,rn,rp.domain,rp.port);
//        }
        std::cout << "fd " << fd << std::endl;
        while ((rn = read(fd, sdbuf, 4096)) > 0) {
            // std::cout<<i++<<std::endl;
            //   std::cout<<"Regex"<<std::endl;
            GetUrl(sdbuf, rn, rp.domain, rp.port);
        }
        close(fd);

        std::cout << "close fd " << fd << std::endl;

        sleep(1);
    }
    return nullptr;
}


void UrlParse::Init(std::string url) {

    do {
        MutexRAII<pthread_mutex_t> loc(mutex_ipMap);
        ipMap.clear();
    } while (0);
    do {
        MutexRAII<pthread_mutex_t> loc(mutex_Set_Url);
        Set_Url.clear();
    } while (0);

    do {
        MutexRAII<pthread_mutex_t> loc(mutex_Que_RegexFile);
        while (!Que_RegexFile.empty()) {
            Que_RegexFile.pop();
        }
    } while (0);


    pthread_t tid;
    int terror;
    terror = pthread_create(&tid, NULL, RegexThreadFunc, NULL);

    if (terror != 0) {
        Log::unix_error("InitRegexThread ");
    }


    struct Url u;
    std::tuple<std::string, std::string, std::string> res = ToParseUrl(url);
    u.url = url;
    u.domain = std::get<0>(res);
    if (!std::get<1>(res).empty())
        u.port = std::stoi(std::get<1>(res));
    else
        u.port = 80;
    u.path = std::get<2>(res);
    u.state = 0;
    u.dep = 0;
    if (AddUrl(u) != 0) {
        Log::unix_error("init erroe");
    }

    int timeout=0;
    for(;timeout<100;) {
        sleep(10);
//        do {
//            MutexRAII<pthread_mutex_t> lock(mutex_ipMap);
//
//
//            for (auto &i:ipMap) {
//
//                do{
//                    MutexRAII<pthread_mutex_t> lckn(mutex_threadNum);
//                    if(threadNum<30)
//                    {
//                        threadNum++;
//                    }else {
//                        goto loop1;
//                    }
//                }while(0);
//
//
//                if (i.second.empty()) {
//                    continue;
//                }
//
//                pthread_t tid;
//                int terror;
//                // std::cout<<"create thread "<<tid <<" domainhash="<<domainHash <<std::endl;
//                terror = pthread_create(&tid, NULL, SpiderThreadFunc, new size_t(i.first));
//
//                if (terror != 0) {
//                    Log::unix_error("create thread in fix");
//                }else{
//                    std::cout<<"create thread "<<tid <<" domainhash="<<i.first <<std::endl;
//                }
//
//                 loop1:;
//            }
//        } while (0);
    }
    return;
}