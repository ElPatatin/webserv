/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sock.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:37:28 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/17 15:58:18 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_HPP
# define SOCK_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <cerrno>
# include <csignal>
# include <exception>
# include <sys/socket.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/epoll.h>

# define MAX_EVENTS 10
# define TIMEOUT_MS 10000

class Sock
{
    public:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================

        Sock( int domain, int service, int protocol, u_int16_t port );
        static void handleSignal( int signal );
        ~Sock( );

    private:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================

        Sock( );
        Sock( Sock const & src );

        // OPERATORS OVERLOAD
        // ==================

        Sock & operator=( Sock const & rhs );

        // METHODS
        // =======


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

        class SocketSetOptionFailed : public std::runtime_error
        {
            public:
                SocketSetOptionFailed( std::string const & msg );
        };

        class SocketEpollFailed : public std::runtime_error
        {
            public:
                SocketEpollFailed( std::string const & msg );
        };

        class SocketEpollCtlFailed : public std::runtime_error
        {
            public:
                SocketEpollCtlFailed( std::string const & msg );
        };

        class SocketEpollWaitFailed : public std::runtime_error
        {
            public:
                SocketEpollWaitFailed( std::string const & msg );
        };

        class SocketEpollCloseFailed : public std::runtime_error
        {
            public:
                SocketEpollCloseFailed( std::string const & msg );
        };

        class SocketRecieveFailed : public std::runtime_error
        {
            public:
                SocketRecieveFailed( std::string const & msg );
        };

        class SocketSendFailed : public std::runtime_error
        {
            public:
                SocketSendFailed( std::string const & msg );
        };

        // ATTRIBUTES
        // ==========

        int _conn_fd;
        int _new_conn_fd;
        int _addr_len;
        struct sockaddr_in _addr;
        int _buffer[1024];

        static volatile bool _keep_running;
};

#endif
