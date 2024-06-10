/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sock.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:47:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/10 12:57:19 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sock.hpp"

Sock::Sock( void ) { return ; }

Sock::Sock( Sock const & src ) { (void)src; return ; }

Sock::~Sock( void ) { return ; }

Sock & Sock::operator=( Sock const & rhs ) { (void)rhs; return *this; }

Sock::Sock( int domain, int service, int protocol, int port )
{
    if ((this->_conn_fd = socket( domain, service, protocol )) && this->_conn_fd == -1)
        throw Sock::SocketCreationFailed( "Socket creation failed" );

    this->_addr.sin_family = domain;
    this->_addr.sin_port = htons( port );
    this->_addr.sin_addr.s_addr = INADDR_ANY;
    std::memset( this->_addr.sin_zero, '\0', sizeof( this->_addr.sin_zero ) );

    if (bind( this->_conn_fd, reinterpret_cast<struct sockaddr *>(&this->_addr), sizeof( this->_addr )) == -1)
        throw Sock::SocketBindFailed( "Socket bind failed" );

    return ;
}