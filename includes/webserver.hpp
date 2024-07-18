/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:31:58 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/18 18:04:44 by cpeset-c         ###   ########.fr       */
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
# include <fcntl.h>

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
    int         listen_sock;
    int         conn_sock;
    int         conn_fd;
    SockAddrIn  addr;
    size_t      addr_len;
    std::string response;
}   Data;

typedef struct s_epoll
{
    int         epoll_fd;
    int         nfds;
    EpollEvent  event;
    EpollEvent  events[ MAX_EVENTS ];
}   EpollData;

namespace g_signal { extern volatile sig_atomic_t g_signal_status; }

// Signals
void    signalHandler( int signum );

// Webserver
void    webserver( Cluster & cluster );

namespace Sockets
{
    Addrs   resolveHostToIp( int domain, int type, std::string host );
    int     createSocket( AddrInfo *rp );
    void    bindSocket( Data *data, ConfigData & config );
    void    listenConnection( Data & data, int backlog );
    void    acceptConnection( Data & data );
    void    setSocketBlockingMode( int sockfd, bool blocking );
    void    receiveConnection( Data & data, ConfigData & config );
    bool    headersReceived( const std::string & request, int & content_length );
    void    sendConnection( Data & data );
    void    closeConnection( int fd, std::string function, int line );
}

namespace Epoll
{
    void    createEpoll( EpollData & epoll );
    void    addEpoll( EpollData & epoll, int sock, int events );
    void    waitEpoll( EpollData & epoll );
}

#endif