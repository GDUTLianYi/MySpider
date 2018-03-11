//
// Created by lianyi on 18-3-10.
//

#include "TaskQueueSelect.h"
//
// Created by lianyi on 18-2-25.
//

#include "CSocket.h"
#include"Http.h"
#include "Log.h"
#include "Rio.h"
#include<error.h>
#include"FileHelper.h"

#define MODE S_IRUSR|S_IWUSR|S_IXUSR

#include <sys/select.h>

#define    F_CONNECTING      1    /* connect() in progress */
#define    F_READING         2    /* connect() complete; now reading */
#define    F_BODY            3
#define    F_DONE            4    /* all done */
char buf[RIO_BUFSIZE];
char sdbuf[1024];

struct file {
    int f_fd;                /* descriptor */
    int f_flags;
    struct Url t;
} file[20];

fd_set rset, wset;
int maxfd = 0;


int TaskQueueSelect::Run() {
    Rio m_rio;
    int i = 0;
    FD_ZERO(&wset);
    FD_ZERO(&rset);
    for (int i = 0; i < 20; i++) {
        file[i].f_flags = 0;
    }
    int which = 0;
    while (!m_UrlParse.Empty()&&which<200) {



        for (int i = 0; i < 20; i++) {
            if (!(file[i].f_flags == 0 || file[i].f_flags == F_DONE)) {
                continue;
            }
            int fid = i;

            if (m_UrlParse.Empty()) {

                break;
            }else{
                which++;
            }
            file[fid].t = m_UrlParse.pop();

            /*
             * 进行无阻塞connect操作
             */
            int clientfd;
            if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                Log::unix_error("create socket error");
                continue;
            }
            file[fid].f_fd = clientfd;

            int flags = fcntl(clientfd, F_GETFD, 0);
            fcntl(clientfd, F_SETFD, flags | O_NONBLOCK);

            struct sockaddr_in serveraddr;
            bzero(&serveraddr, sizeof(serveraddr));
            serveraddr.sin_family = AF_INET;
            serveraddr.sin_port = htons(file[fid].t.port);
            inet_pton(AF_INET, file[fid].t.ip.c_str(), &serveraddr.sin_addr);


            if (connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
                if (errno != EINPROGRESS) {
                    Log::unix_error("noblock connect error");
                    close(clientfd);
                    continue;
                } else {
                    file[fid].f_flags = F_CONNECTING;
                    if (clientfd > maxfd) {
                        maxfd = clientfd;
                    }
                    /*
                     * 设置可读可写
                     */
                    FD_SET(clientfd, &rset);
                    FD_SET(clientfd, &wset);
                }
            } else {
                file[fid].f_flags = F_READING;
                if (clientfd > maxfd) {
                    maxfd = clientfd;
                }
                Http::Form_Get(sdbuf, file[fid].t);
                if (send(file[fid].f_fd, sdbuf, strlen(sdbuf), 0) == -1) {
                    Log::unix_error("send get error");
                    close(file[fid].f_fd);

                    continue;
                }
                std::cout << sdbuf << std::endl;
                /*
               * 设置可读
               */
                FD_SET(clientfd, &rset);
            }
        }
        fd_set rs, ws;
        rs = rset, ws = wset;
        int n;
        n = select(maxfd + 1, &rs, &ws, NULL, NULL);

        for (int i = 0; i < 20; i++) {
            int tmpflags = file[i].f_flags;
            if (tmpflags == 0 || (tmpflags & F_DONE)) {
                continue;
            }

            int tmpfd = file[i].f_fd;

            if (tmpflags & F_CONNECTING && (FD_ISSET(tmpfd, &rs) || FD_ISSET(tmpfd, &ws))) {
                FD_CLR(tmpfd, &wset);
                FD_CLR(tmpfd, &rset);
                file[i].f_flags = F_DONE;
                /*
                 * 清除可读可写
                 */
                int error;
                n = sizeof(error);
                if (getsockopt(tmpfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *) &n) < 0 || error != 0) {
                    Log::unix_error("nonblock connect failure");
                    continue;
                }
                Http::Form_Get(sdbuf, file[i].t);
                if (send(file[i].f_fd, sdbuf, strlen(sdbuf), 0) == -1) {
                    Log::unix_error("send get error");
                    continue;
                }

                file[i].f_flags = F_READING;
                FD_SET(tmpfd, &rset);
                /*
                 * 设置可读
                 */
            } else if (tmpflags & F_READING && FD_ISSET(tmpfd, &rs)) {
                /*
                 * 清除可读
                 */
                file[i].f_flags = F_DONE;
                FD_CLR(tmpfd, &rset);
                int tmpfd = file[i].f_fd;

                ssize_t rn = 0;
                int state = -1;
                char pro[20], res[20];

                rio_t rio;
                m_rio.readinitb(&rio, tmpfd);
                while ((rn = m_rio.readlineb(&rio, buf, RIO_BUFSIZE)) > 0) {
                    std::cout << buf;
                    if (state == -1) {
                        sscanf(buf, "%s %d %s", pro, &state, res);
                        if (state != 200) {
                            break;
                        }
                    }
                    if (buf[0] == '\r' && buf[1] == '\n') {
                        break;
                    }
                }

                std::string DirPath, FilePath;
                FileHelper::getDirAndFileName(file[i].t, DirPath, FilePath);
                FileHelper::CreateDir(DirPath.c_str());
                int Fd_SaveFile = open(FilePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, MODE);
                if (Fd_SaveFile == -1) {
                    close(tmpfd);
                    Log::unix_error("create open file error");
                    continue;
                }

                while ((rn = m_rio.readnb(&rio, buf, RIO_BUFSIZE)) > 0) {
                    // std::cout<<i++<<std::endl;
                    m_UrlParse.GetUrl(buf, rn, file[i].t.domain, file[i].t.port);
                    //  std::cout<<"hehe"<<std::endl;
                    write(Fd_SaveFile, buf, rn);
                }
                close(tmpfd);
                close(Fd_SaveFile);
            }

        }
    }
}

int TaskQueueSelect::init(std::string url) {
    struct Url u;
    std::tuple<std::string, std::string, std::string> res = m_UrlParse.ToParseUrl(url);
    u.url = url;
    u.domain = std::get<0>(res);
    if (!std::get<1>(res).empty())
        u.port = std::stoi(std::get<1>(res));
    else
        u.port = 80;
    u.path = std::get<2>(res);
    u.state = 0;
    u.dep = 0;
    if (!m_UrlParse.AddUrl(u)) {
        Log::unix_error("init erroe");
    }
}