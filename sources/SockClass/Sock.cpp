/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sock.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:47:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/10 18:15:58 by alajara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sock.hpp"
#include "colors.hpp"

Sock::Sock( void ) { return ; }

Sock::Sock( Sock const & src ) { (void)src; return ; }

Sock::~Sock( void ) { return ; }

Sock & Sock::operator=( Sock const & rhs ) { (void)rhs; return *this; }

Sock::Sock( int domain, int service, int protocol, int port )
{
    this->_addr_len = sizeof( this->_addr );

    if ((this->_conn_fd = socket( domain, service, protocol )) < 0)
        throw Sock::SocketCreationFailed( "Socket creation failed" );

    this->_addr.sin_family = domain;
    this->_addr.sin_port = htons( port );
    this->_addr.sin_addr.s_addr = INADDR_ANY;
    std::memset( this->_addr.sin_zero, '\0', sizeof( this->_addr.sin_zero ) );

    if ( bind( this->_conn_fd, (struct sockaddr *)(& _addr ), _addr_len) < 0 )
        throw Sock::SocketBindFailed( "Socket bind failed" );

    if ( listen( this->_conn_fd, 10 ) < 0 )
        throw Sock::SocketListenFailed( "Socket listen failed" );
    
    while (42)
    {
        std::cout << ORANGE << "Waiting for connection..." << RESET << std::endl;

        if ( ( this->_new_conn_fd = accept( this->_conn_fd, \
            reinterpret_cast<struct sockaddr *>( &this->_addr ), \
            reinterpret_cast<socklen_t *>( &this->_addr_len ) ) ) < 0 )
            throw Sock::SocketAcceptFailed( "Socket accept failed" );

        std::cout << GREEN << "Connection accepted" << RESET << std::endl;

        char buffer[1024] = {0};
        int valread = read( this->_new_conn_fd, buffer, 1024 );
        if (valread < 0)
            throw Sock::SocketAcceptFailed( "Socket accept failed 2 " );
        if ( write( this->_new_conn_fd, buffer, valread ) < 0 )
            throw Sock::SocketAcceptFailed( "Socket accept failed 3" );
 
        if ( close( this->_new_conn_fd ) < 0 )
            throw Sock::SocketCloseFailed( "Socket close failed" );
    }

    return ;
}

struct sockaddr_in Sock::getAddr( void ) const { return this->_addr; }
