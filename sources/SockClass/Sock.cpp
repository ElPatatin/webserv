/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sock.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:47:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/17 00:00:05 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sock.hpp"
#include "colors.hpp"

volatile bool Sock::_keep_running = true;
int static_conn_fd = 0;

Sock::Sock( void ) { return ; }

Sock::Sock( Sock const & src ) { (void)src; return ; }

Sock::~Sock( void ) { return ; }

Sock & Sock::operator=( Sock const & rhs ) { (void)rhs; return *this; }

Sock::Sock( int domain, int service, int protocol, int port )
{
    // Create a socket
    if ((this->_conn_fd = socket( domain, service, protocol )) < 0)
        throw SocketCreationFailed( "Socket creation failed" );

    static_conn_fd = this->_conn_fd;

    // Bind the socket
    this->_addr_len = sizeof( this->_addr );

    this->_addr.sin_family = domain;
    this->_addr.sin_port = htons( port );
    this->_addr.sin_addr.s_addr = INADDR_ANY;
    std::memset( this->_addr.sin_zero, '\0', sizeof( this->_addr.sin_zero ) );


    int opt = 1;
    if (setsockopt(this->_conn_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw SocketSetOptionFailed("Setsockopt SO_REUSEADDR failed");


    if ( bind( this->_conn_fd, reinterpret_cast< struct sockaddr * >( & this->_addr ), sizeof( this->_addr ) ) < 0 )
        throw SocketBindFailed( "Socket bind failed" );

    // Listen for connections
    if ( listen( this->_conn_fd, 10 ) < 0 )
        throw SocketListenFailed( "Socket listen failed" );

    // Create epoll instance
    int epoll_fd = epoll_create( 1 );
    if ( epoll_fd == -1 )
        throw SocketEpollFailed( "Failed to create epoll instance" );

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = this->_conn_fd;

    // Add the socket to the epoll instance
    if ( epoll_ctl( epoll_fd, EPOLL_CTL_ADD, this->_conn_fd, &event ) == -1 )
    {
        if ( close( epoll_fd ) < 0 )
            throw SocketEpollCloseFailed( "Failed to close epoll file descriptor" );
        throw SocketEpollCtlFailed( "Failed to add file descriptor to epoll" );
    }

    struct epoll_event events[MAX_EVENTS];

    // Accept connections
    while ( this->_keep_running )
    {
        std::cout << ORANGE << "Waiting for connection..." << RESET << std::endl;

        // Wait for events on the epoll instance
        int nfds = epoll_wait( epoll_fd, events, MAX_EVENTS, TIMEOUT_MS );
        if ( nfds == -1 )
        {
            if ( errno == EINTR )
                continue ;
            else
            {
                if ( close( epoll_fd ) < 0 )
                    throw SocketEpollCloseFailed( "Failed to close epoll file descriptor" );
                throw SocketEpollWaitFailed( "epoll_wait failed" );
            }
        }

        // Timeout occurred
        if ( nfds == 0 )
        {
            std::cout << "Timeout occurred! No connection requests." << std::endl;

            if ( close( epoll_fd ) < 0 )
                throw SocketEpollCloseFailed( "Failed to close epoll file descriptor" );
            if ( close( this->_conn_fd ) < 0 )
                throw SocketCloseFailed( "Failed to close socket" );

            std::cout << GREEN << "Socket closed" << RESET << std::endl;
            return ;
        }
        
        for ( int n = 0; n < nfds; ++n )
        {
            if ( events[n].data.fd == this->_conn_fd )
            {
                // Accept a connection
                if ( ( this->_new_conn_fd = accept( this->_conn_fd, \
                    reinterpret_cast<struct sockaddr *>( &this->_addr ), \
                    reinterpret_cast<socklen_t *>( &this->_addr_len ) ) ) < 0 )
                {
                    if ( this->_keep_running )
                        throw SocketAcceptFailed( "Socket accept failed" );
                    break ;
                }

                std::cout << GREEN << "Connection accepted" << RESET << std::endl;

                // Receive data
                std::memset( this->_buffer, '\0', sizeof( this->_buffer ) );
                if ( recv( this->_new_conn_fd, this->_buffer, 1024, 0 ) < 0 )
                    throw SocketRecieveFailed( "Failed to receive data" );

                std::cout << "Received: " << this->_buffer << std::endl;

                // Send data
                if ( send( this->_new_conn_fd, this->_buffer, 1024, 0 ) < 0 )
                    throw SocketSendFailed( "Failed to send data" );
            }
        }
    }

    if ( close( epoll_fd ) < 0 )
        throw SocketEpollCloseFailed( "Failed to close epoll file descriptor" );
    std::cout << ORANGE << "Shutting down..." << RESET << std::endl;

    return ;
}

// Signal handler to set the flag to false
void Sock::handleSignal( int signal )
{
    _keep_running = false;

    std::cout << "\n\033[A\033[K";
    if ( signal == SIGINT )
        std::cout << ORANGE << "SIGINT received" << RESET << std::endl;
    else if ( signal == SIGTERM )
        std::cout << ORANGE << "SIGTERM received" << RESET << std::endl;
    else if ( signal == SIGQUIT )
        std::cout << ORANGE << "SIGQUIT received" << RESET << std::endl;

    if ( close( static_conn_fd ) < 0 )
        throw Sock::SocketCloseFailed( "Socket close failed" );
    std::cout << GREEN << "Socket closed" << RESET << std::endl;
    return ;
}

struct sockaddr_in Sock::getAddr( void ) const { return this->_addr; }
