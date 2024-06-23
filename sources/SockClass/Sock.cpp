/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sock.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:47:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 18:09:52 by cpeset-c         ###   ########.fr       */
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

Sock::Sock( int domain, int service, int protocol, u_int16_t port, std::string host )
{
    // Resolve host to IP address
    struct addrinfo hints, *servinfo, *p;
    std::memset(&hints, 0, sizeof hints);
    hints.ai_family = domain;
    hints.ai_socktype = service;

    UNUSED(protocol);

    int rv;
    if ( (rv = getaddrinfo( host.c_str(), NULL, &hints, &servinfo ) ) != 0 )
        throw std::runtime_error( gai_strerror( rv ) );

    // Loop through all the results and bind to the first we can
    for ( p = servinfo; p != NULL; p = p->ai_next )
    {
        if ( ( this->_conn_fd = socket( p->ai_family, p->ai_socktype, p->ai_protocol) ) == -1 )
                throw SocketCreationFailed( "Socket creation failed" );

        static_conn_fd = this->_conn_fd;

        // Bind the socket
        this->_addr_len = sizeof( this->_addr );
        this->_addr.sin_family = domain;
        this->_addr.sin_port = htons( port );
        this->_addr.sin_addr.s_addr = htonl( INADDR_ANY );
        std::memset( this->_addr.sin_zero, '\0', sizeof( this->_addr.sin_zero ) );


        int opt = 1;
        if (setsockopt(this->_conn_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            throw SocketSetOptionFailed("Setsockopt SO_REUSEADDR failed");
        
        if ( bind( this->_conn_fd, reinterpret_cast< struct sockaddr * >( & this->_addr ), sizeof( this->_addr ) ) < 0 )
            throw SocketBindFailed( "Socket bind failed" );

        break;
    }

    if ( p == NULL )
        throw SocketBindFailed( "Failed to bind socket" );

    freeaddrinfo( servinfo );
    
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

                this->handleRequest( this->_new_conn_fd );

                if ( close( this->_new_conn_fd ) < 0 )
                    throw SocketCloseFailed( "Failed to close socket" );
            }
        }
    }

    if ( close( epoll_fd ) < 0 )
        throw SocketEpollCloseFailed( "Failed to close epoll file descriptor" );
    std::cout << ORANGE << "Shutting down..." << RESET << std::endl;

    return ;
}


void Sock::handleRequest(int client_fd)
{
    // Receive HTTP request
    std::memset(this->_buffer, '\0', sizeof(this->_buffer));
    if (recv(client_fd, this->_buffer, sizeof(this->_buffer) - 1, 0) < 0)
        throw SocketRecieveFailed("Failed to receive data");
        
    // Print received request to standard output
    LOG( DEBUG3 ) << "Received HTTP request:";
    LOG( DEBUG3 ) << this->_buffer;

    // Parse HTTP request (very basic parsing)
    std::string request(this->_buffer);
    std::istringstream request_stream(request);
    std::string method, path, version;
    request_stream >> method >> path >> version;

    if (method == "GET" && path == "/")
    {
        // Serve the index.html file
        std::fstream file;

        file.open( "./html/index.html", std::ios::in );
        
        if (!file.is_open())
            return this->serveErrorNotFound( client_fd );
            

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.length() << "\r\n"
                        << "Content-Type: text/html\r\n"
                        << "\r\n"
                        << content;

        std::string response = response_stream.str();
        send(client_fd, response.c_str(), response.length(), 0);
    }
    else if ( method == "GET" && path == "/favicon.ico" )
    {
         // Serve the favicon.ico file
        std::ifstream file("./html/favicon.ico", std::ios::in | std::ios::binary);
        
        if (!file.is_open())
            return this->serveErrorNotFound( client_fd );

        std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.size() << "\r\n"
                        << "Content-Type: image/x-icon\r\n"
                        << "\r\n";

        std::string header = response_stream.str();
        send(client_fd, header.c_str(), header.length(), 0);
        send(client_fd, content.data(), content.size(), 0);
    }
    else if ( method == "GET" )
    {
        return this->serveErrorNotFound( client_fd );
    }
    else
    {
        // Method not supported or not found
        std::string response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
        send(client_fd, response.c_str(), response.length(), 0);
    }
}

void Sock::serveErrorNotFound( int client_fd )
{
    std::fstream file_not_foun; 
    file_not_foun.open( "./html/errors/404.html", std::ios::in );
    std::string content( ( std::istreambuf_iterator<char>( file_not_foun ) ), std::istreambuf_iterator<char>() );
    file_not_foun.close();

    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 404 Not Found\r\n"
                    << "Content-Length: " << content.length() << "\r\n"
                    << "Content-Type: text/html\r\n"
                    << "\r\n"
                    << content;
    
    std::string response = response_stream.str();
    send(client_fd, response.c_str(), response.length(), 0);
    return;
}

// Signal handler to set the flag to false
void Sock::handleSignal( int signal )
{
    _keep_running = false;

    std::cout << "\n\033[A\033[K";
    if ( signal == SIGINT )
        std::cout << ORANGE << "SIGINT received" << RESET << std::endl;
    else if ( signal == SIGQUIT )
        std::cout << ORANGE << "SIGQUIT received" << RESET << std::endl;

    if ( close( static_conn_fd ) < 0 )
        throw Sock::SocketCloseFailed( "Socket close failed" );
    std::cout << GREEN << "Socket closed" << RESET << std::endl;
    return ;
}

struct sockaddr_in Sock::getAddr( void ) const { return this->_addr; }
