/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SockException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:56:08 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/16 14:00:49 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sock.hpp"

// EXCEPTIONS
// ==========

Sock::SocketCreationFailed::SocketCreationFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketBindFailed::SocketBindFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketListenFailed::SocketListenFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketAcceptFailed::SocketAcceptFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketCloseFailed::SocketCloseFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketSetOptionFailed::SocketSetOptionFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }