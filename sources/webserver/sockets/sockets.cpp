/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:40:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/27 20:33:41 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

Addrs    Sockets::resolveHostToIp( int domain, int type, std::string host )
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

int Sockets::createSocket( AddrInfo *rp )
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

void    Sockets::bindSocket( Data *data, ConfigData config )
{
    data->addr.sin_family = AF_INET;
    data->addr.sin_port = htons( config.getPort() );
    data->addr.sin_addr.s_addr = htonl( INADDR_ANY );
    std::memset( data->addr.sin_zero, '\0', sizeof( data->addr.sin_zero ) );

    if ( bind( data->conn_fd, reinterpret_cast< SockAddr * >( &data->addr ), sizeof( data->addr ) ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "bind: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: bind: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}