#include "LoadConfig.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void test(std::map<std::string, FieldInterface *> config)
{
    UNUSED(config);

    struct sockaddr_in addr;
    socklen_t addrlen;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    addrlen = sizeof(addr);

    struct epoll_event ev, events[10];
    int listen_sock, conn_sock, nfds, epollfd;

    epollfd = epoll_create(1);
    if (epollfd == -1)
        throw std::runtime_error("epoll_create1");
    
    ev.events = EPOLLIN;
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1)
        throw std::runtime_error("socket");
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
        throw std::runtime_error("epoll_ctl: listen_sock");
    
    while (42)
    {
        nfds = epoll_wait(epollfd, events, 10, -1);
        if (nfds == -1)
            throw std::runtime_error("epoll_wait");
        
        for (int n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == listen_sock)
            {
                conn_sock = accept(listen_sock, (struct sockaddr *) &addr, &addrlen);
                if (conn_sock == -1)
                    throw std::runtime_error("accept");
                
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
                    throw std::runtime_error("epoll_ctl: conn_sock");
            }
            else
            {
                write(events[n].data.fd, "Hello, World!\n", 14);
                close(events[n].data.fd);

            }
        }
    }
}