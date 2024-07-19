/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets_conn.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:40:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/19 19:41:42 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"

Addrs    ConectionSockets::resolveHostToIp( int domain, int type, std::string host )
{
    AddrInfo    hints;
    Addrs       addrs;

    std::memset( &hints, 0, sizeof( AddrInfo ) );
    hints.ai_family = domain;
    hints.ai_socktype = type;

    int rv;
    if ( ( rv = getaddrinfo( host.c_str(), NULL, &hints, &addrs.result ) ) != 0 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "getaddrinfo: " + std::string( gai_strerror( rv ) ) );
        throw ResolveHostException( "Error: getaddrinfo: " + std::string( gai_strerror( rv ) ) );
    }

    addrs.rp = addrs.result;
    return ( addrs );
}

int ConectionSockets::createSocket( AddrInfo *rp )
{
    int conn_fd;

    if ( ( conn_fd = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol ) ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "socket: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: socket: " + std::string( std::strerror( errno ) ) );
    }

    int opt = 1;
    if ( setsockopt( conn_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( int ) ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "setsockopt: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: setsockopt: " + std::string( std::strerror( errno ) ) );
    }

    return ( conn_fd );
}

void    ConectionSockets::bindSocket( Data *data, ConfigData & config )
{
    data->addr_len = sizeof( data->addr );
    data->addr.sin_family = AF_INET;
    data->addr.sin_port = htons( config.getPort() );
    data->addr.sin_addr.s_addr = htonl( INADDR_ANY );
    std::memset( data->addr.sin_zero, '\0', sizeof( data->addr.sin_zero ) );

    // Assigns the address specified by addr to the socket referred to by the file descriptor conn_fd.
    if ( bind( data->listen_sock, reinterpret_cast< SockAddr * >( &data->addr ), static_cast< socklen_t >( data->addr_len ) ) == -1 )  
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "bind: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: bind: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    ConectionSockets::listenConnection( Data & data, int backlog )
{
    // Marks the socket referred to by conn_fd as a passive socket, that is,
    // as a socket that will be used to accept incoming connection requests using accept.
    if ( listen( data.listen_sock, backlog ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "listen: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: listen: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    ConectionSockets::acceptConnection( Data & data )
{
    LOG( INFO ) << "Accepting connection";
    // It extracts the first connection request on the queue of pending connections for
    // the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket.
    if ( ( data.conn_sock = accept( data.listen_sock,\
            reinterpret_cast< SockAddr * >( &data.addr ),\
            reinterpret_cast< socklen_t * >( &data.addr_len ) ) ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "accept: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: accept: " + std::string( std::strerror( errno ) ) );
    }


    return ;
}

void    ConectionSockets::setSocketBlockingMode( int sockfd, bool blocking )
{
    int flags = fcntl( sockfd, F_GETFL, 0 );
    if ( flags == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "fcntl: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: fcntl: " + std::string( std::strerror( errno ) ) );
    }

    if ( blocking )
        flags &= ~O_NONBLOCK; // Clear O_NONBLOCK flag
    else
        flags |= O_NONBLOCK; // Set O_NONBLOCK flag

    if ( fcntl( sockfd, F_SETFL, flags ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "fcntl: " + std::string( std::strerror( errno)));
        throw SocketException("Error: fcntl: " + std::string(std::strerror(errno)));
    }

    return ;
}

void    ConectionSockets::closeConnection( int fd , std::string function, int line )
{
    if ( close( fd ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( function, line, "close: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: close: " + std::string( std::strerror( errno ) ) );
    }
    return ;
}