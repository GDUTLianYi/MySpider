//
// Created by lianyi on 18-2-27.
//

#include "UrlParse.h"
#include "FileHelper.h"

#define MODE S_IRUSR|S_IWUSR|S_IXUSR


extern int FinshSign;
extern pthread_mutex_t mutex_Que_UrlBeforeDns ;
extern std::queue<Url> Que_UrlBeforeDns;
extern void  * ThreadSocketFunc(void *argc);
extern void  * ThreadDnsFunc(void *argc);
extern pthread_mutex_t mutex_Que_UrlBeforeConnect;
extern std::queue<Url> Que_UrlBeforeConnect;
pthread_mutex_t mutex_ipMap = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Que_RegexFile = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_Set_Url = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_threadNum = PTHREAD_MUTEX_INITIALIZER;


std::unordered_set<std::size_t> Set_Url;
std::tuple<std::string, std::string, std::string> ToParseUrl(std::string url);
int threadNum = 0;

std::size_t GetHashcode(std::string s) {
    return std::hash<std::string>{}(s);
}

typedef std::unordered_set<struct Url, UrlKey, UrlCmp> urlSet;
std::unordered_map<std::size_t, urlSet> ipMap;
std::queue<RegexPackage> Que_RegexFile;

int xxxx = 1;

int AddUrl(struct Url &url) {


    std::size_t hashurl = GetHashcode(url.url);
    do {
        //  std::cout<<"apply mutex_Set_Url"<<std::endl;
        MutexRAII<pthread_mutex_t> lcks(mutex_Set_Url);
        //std::cout<<"apply mutex_Set_Url OK "<<std::endl;
        if (Set_Url.count(hashurl)) {
            return 1;
        } else {
            Set_Url.insert(hashurl);
        }
    } while (0);

    do {
        //  std::cout<<"apply mutex_Set_Url"<<std::endl;
        MutexRAII<pthread_mutex_t> lcks(mutex_Que_UrlBeforeDns);
        //std::cout<<"apply mutex_Set_Url OK "<<std::endl;
        Que_UrlBeforeDns.push(url);
       // std::cout<<"Que_UrlBeforeDns size="<<Que_UrlBeforeDns.size()<<std::endl;

    } while (0);

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


void *RegexThreadFunc(void *argc) {


    Rio m_rio;
    rio_t rio;
    char sdbuf[8192];
    RegexPackage rp;
    int flag;
    int emp=0;
    for (; FinshSign != 1;) {
        if (emp >0) {
            sleep(10+5*emp);
            emp=0;
        }

        if(1) {
            MutexRAII<pthread_mutex_t> lockf(mutex_Que_RegexFile);
            if (Que_RegexFile.empty()) {
                //std::cout<<"sleep"<<std::endl;
                emp = 1;
                continue;
            }
            rp = Que_RegexFile.front();
            Que_RegexFile.pop();
            // std::cout<<"take out from queRgex"<<std::endl;
        }

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
            //  std::cout<<"Regex"<<std::endl;
            GetUrl(sdbuf, rn, rp.domain, rp.port);
        }
        close(fd);
        std::cout << "close fd " << fd << std::endl;


        if (1) {
            MutexRAII<pthread_mutex_t> lockf(mutex_Que_UrlBeforeDns);
            if(Que_UrlBeforeDns.size()>100) {
                std::cout << "Que_UrlBeforeDns size=" << Que_UrlBeforeDns.size() << std::endl;
                emp = Que_UrlBeforeDns.size()/20;
            }
        }

        if (1) {
            MutexRAII<pthread_mutex_t> lockf(mutex_Que_UrlBeforeConnect);
            std::cout << "Que_UrlBeforeConnect size=" << Que_UrlBeforeConnect.size()<< std::endl;
        }

        if (1) {
            MutexRAII<pthread_mutex_t> lockf(mutex_ipMap);
            std::cout << "ipMap size=" << ipMap.size() << std::endl;
        }
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

    ThreadPool threadPool(2);
    threadPool.Init(2);

    pthread_t sockettid;
    terror= pthread_create(&sockettid,NULL,ThreadSocketFunc,NULL);
    if(terror!=0){
        Log::unix_error("create ThreadSocketFunc error");
    }else{
        std::cout<<"create ThreadSocketFunc success "<<tid<<std::endl;
    }

    pthread_t dnstid;
    terror= pthread_create(&dnstid,NULL,ThreadDnsFunc,NULL);
    if(terror!=0){
        Log::unix_error("create ThreadDnsFunc error");
    }else{
        std::cout<<"create ThreadDnsFunc success "<<tid<<std::endl;
    }

    int timeout = 0;
    for (; timeout < 100;) {
        sleep(10);
    }
    return;
}