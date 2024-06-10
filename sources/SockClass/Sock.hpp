/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sock.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:37:28 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/10 15:30:07 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_HPP
# define SOCK_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <sys/socket.h>
#include <unistd.h>
// #include <netinet/in.h>
#include <arpa/inet.h>

class Sock
{
    public:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================

        Sock( int domain, int service, int protocol, int port );
        ~Sock( );

    private:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================

        Sock( );
        Sock( Sock const & src );

        // OPERATORS OVERLOAD
        // ==================

        Sock & operator=( Sock const & rhs );

        // ACCESSORS
        // =========

        struct sockaddr_in getAddr( void ) const;

        // EXCEPTIONS
        // ==========

        class SocketCreationFailed : public std::runtime_error
        {
            public:
                SocketCreationFailed( std::string const & msg );
        };

        class SocketBindFailed : public std::runtime_error
        {
            public:
                SocketBindFailed( std::string const & msg );
        };

        class SocketListenFailed : public std::runtime_error
        {
            public:
                SocketListenFailed( std::string const & msg );
        };

        class SocketAcceptFailed : public std::runtime_error
        {
            public:
                SocketAcceptFailed( std::string const & msg );
        };

        class SocketCloseFailed : public std::runtime_error
        {
            public:
                SocketCloseFailed( std::string const & msg );
        };

        // ATTRIBUTES
        // ==========

        int _conn_fd;
        int _new_conn_fd;
        struct sockaddr_in _addr;
        int _addr_len;
};

#endif
