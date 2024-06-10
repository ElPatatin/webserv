/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sock.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:37:28 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/10 12:57:18 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_HPP
# define SOCK_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <sys/socket.h>
// #include <netinet/in.h>
#include <arpa/inet.h>

class Sock
{
    public:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================

        Sock( int domain, int service, int protocol, int port );

    private:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================

        Sock( );
        Sock( Sock const & src );
        ~Sock( );

        // OPERATORS OVERLOAD
        // ==================

        Sock & operator=( Sock const & rhs );

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

        // ATTRIBUTES
        // ==========

        int _conn_fd;
        struct sockaddr_in _addr;
};

#endif