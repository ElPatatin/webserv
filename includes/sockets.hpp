/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:06:48 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/19 19:40:11 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# include "common.hpp"
# include "ConfigData.hpp"
# include "Exceptions.hpp"
# include "Log.hpp"

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# define BACKLOG 10

typedef struct addrinfo     AddrInfo;
typedef struct sockaddr_in  SockAddrIn;
typedef struct sockaddr     SockAddr;

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

namespace ConectionSockets
{
    Addrs   resolveHostToIp( int domain, int type, std::string host );
    int     createSocket( AddrInfo *rp );
    void    bindSocket( Data *data, ConfigData & config );
    void    listenConnection( Data & data, int backlog );
    void    acceptConnection( Data & data );
    void    closeConnection( int fd, std::string function, int line );
    void    setSocketBlockingMode( int sockfd, bool blocking );
}

namespace CommunicationSockets
{
    void    receiveConnection( Data & data, ConfigData & config );
    bool    headersReceived( const std::string & request, int & content_length );
    void    sendConnection( Data & data );
}

#endif