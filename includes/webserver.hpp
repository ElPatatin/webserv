/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:31:58 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/03 15:23:30 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "common.hpp"
# include "ConfigData.hpp"
# include "Exceptions.hpp"
# include "Log.hpp"
# include "utils.hpp"

# include <csignal>
# include <netdb.h>
# include <sys/epoll.h>

# define UNUSED(x) (void)(x)

# define MAX_EVENTS 64
# define BACKLOG 10
# define TIMEOUT 300000

typedef struct addrinfo     AddrInfo;
typedef struct sockaddr_in  SockAddrIn;
typedef struct sockaddr     SockAddr;
typedef struct epoll_event  EpollEvent;

typedef struct s_addrs
{
    AddrInfo    *result;
    AddrInfo    *rp;
}   Addrs;

typedef struct s_data
{
    int         conn_fd;
    int         new_fd;
    SockAddrIn  addr;
    size_t      addr_len;
}   Data;

typedef struct s_epoll
{
    int         epoll_fd;
    int         nfds;
    EpollEvent  event;
    EpollEvent  *events;
}   EpollData;

namespace g_signal { extern volatile sig_atomic_t g_signal_status; }

// Signals
void    signalHandler( int signum );

// Webserver
void    webserver( ConfigData config );

namespace Sockets
{
    Addrs   resolveHostToIp( int domain, int type, std::string host );
    int     createSocket( AddrInfo *rp );
    void    bindSocket( Data *data, ConfigData config );
    void    listenConnection( Data & data, int backlog );
    void    acceptConnection( Data *data );
    void    receiveConnection( Data *data, ConfigData config );
    void    closeConnection( int fd, std::string function, int line );
}

namespace Epoll
{
    void    createEpoll( Data & data, EpollData & epoll );
    void    addEpoll( Data & data, EpollData & epoll );
    void    removeEpoll( Data & data, EpollData & epoll );
    void    updateEpoll( Data & data, EpollData & epoll );
    void    waitEpoll( Data & data, EpollData & epoll );
}

#endif