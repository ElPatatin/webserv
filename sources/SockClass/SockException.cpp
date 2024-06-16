/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SockException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:56:08 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/16 23:55:26 by cpeset-c         ###   ########.fr       */
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

Sock::SocketEpollFailed::SocketEpollFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketEpollCtlFailed::SocketEpollCtlFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketEpollWaitFailed::SocketEpollWaitFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketEpollCloseFailed::SocketEpollCloseFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketRecieveFailed::SocketRecieveFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }

Sock::SocketSendFailed::SocketSendFailed( std::string const & msg )
    : std::runtime_error( msg ) { return ; }