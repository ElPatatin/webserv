/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sock.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:37:28 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/21 16:14:16 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_HPP
# define SOCK_HPP

# include <csignal>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/epoll.h>
# include <fcntl.h>
# include <sys/types.h>

# include "webserv.hpp"
# include "Exceptions.hpp"

# define MAX_EVENTS 10
# define TIMEOUT_MS 60000

class Sock
{
    public:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================

        Sock( int domain, int service, int protocol, u_int16_t port, std::string host );
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
        
        void handleRequest( int conn_fd );
        void serveErrorNotFound( int conn_fd );


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
        char _buffer[1024];

        static volatile bool _keep_running;
};

#endif
