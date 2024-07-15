/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:40:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/15 14:47:45 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"
#include "http.hpp"

Addrs    Sockets::resolveHostToIp( int domain, int type, std::string host )
{
    AddrInfo    hints;
    Addrs       addrs;

    std::memset( &hints, 0, sizeof( AddrInfo ) );
    hints.ai_family = domain;
    hints.ai_socktype = type;

    int rv;
    if ( ( rv = getaddrinfo( host.c_str(), NULL, &hints, &addrs.result ) ) != 0 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "getaddrinfo: " + std::string( gai_strerror( rv ) ) );
        throw ResolveHostException( "Error: getaddrinfo: " + std::string( gai_strerror( rv ) ) );
    }

    addrs.rp = addrs.result;
    return ( addrs );
}

int Sockets::createSocket( AddrInfo *rp )
{
    int conn_fd;

    if ( ( conn_fd = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol ) ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "socket: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: socket: " + std::string( std::strerror( errno ) ) );
    }

    int opt = 1;
    if ( setsockopt( conn_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( int ) ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "setsockopt: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: setsockopt: " + std::string( std::strerror( errno ) ) );
    }

    return ( conn_fd );
}

void    Sockets::bindSocket( Data *data, ConfigData & config )
{
    data->addr_len = sizeof( data->addr );
    data->addr.sin_family = AF_INET;
    data->addr.sin_port = htons( config.getPort() );
    data->addr.sin_addr.s_addr = htonl( INADDR_ANY );
    std::memset( data->addr.sin_zero, '\0', sizeof( data->addr.sin_zero ) );

    // Assigns the address specified by addr to the socket referred to by the file descriptor conn_fd.
    if ( bind( data->listen_sock, reinterpret_cast< SockAddr * >( &data->addr ), static_cast< socklen_t >( data->addr_len ) ) == -1 )  
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "bind: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: bind: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Sockets::listenConnection( Data & data, int backlog )
{
    // Marks the socket referred to by conn_fd as a passive socket, that is,
    // as a socket that will be used to accept incoming connection requests using accept.
    if ( listen( data.listen_sock, backlog ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "listen: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: listen: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Sockets::acceptConnection( Data & data )
{
    // It extracts the first connection request on the queue of pending connections for
    // the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket.
    if ( ( data.conn_sock = accept( data.listen_sock,\
            reinterpret_cast< SockAddr * >( &data.addr ),\
            reinterpret_cast< socklen_t * >( &data.addr_len ) ) ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "accept: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: accept: " + std::string( std::strerror( errno ) ) );
    }


    return ;
}

void    Sockets::setSocketBlockingMode( int sockfd, bool blocking )
{
    int flags = fcntl( sockfd, F_GETFL, 0 );
    if ( flags == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "fcntl: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: fcntl: " + std::string( std::strerror( errno ) ) );
    }

    if ( blocking )
        flags &= ~O_NONBLOCK; // Clear O_NONBLOCK flag
    else
        flags |= O_NONBLOCK; // Set O_NONBLOCK flag

    if ( fcntl( sockfd, F_SETFL, flags ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "fcntl: " + std::string( std::strerror( errno)));
        throw SocketException("Error: fcntl: " + std::string(std::strerror(errno)));
    }

    return ;
}


void    Sockets::receiveConnection( Data & data, ConfigData & config )
{
    const int buffer_size = 8192;
    char buffer[ buffer_size ];
    std::memset( buffer, '\0', buffer_size );

    std::string full_request;
    int     content_length = 0;
    bool    headers_received = false;
    int     total_bytes_read = 0;

    // Ensure the socket is in blocking mode
    Sockets::setSocketBlockingMode( data.conn_fd, true );

    while ( 42 )
    {
        int bytes_read = recv( data.conn_fd, buffer, buffer_size - 1, 0 );
        if ( bytes_read > 0 )
        {
            buffer[ bytes_read ] = '\0';
            full_request.append( buffer, bytes_read );
            total_bytes_read += bytes_read;

            if ( !headers_received )
                headers_received = headersReceived( full_request, content_length );

            size_t header_end_pos = full_request.find( "\r\n\r\n" );
            if ( headers_received && header_end_pos != std::string::npos )
            {
                size_t body_start_pos = header_end_pos + 4;
                size_t remaining_body_length = content_length - ( total_bytes_read - body_start_pos );

                if ( remaining_body_length <= 0 )
                    break ; // Fully received headers and body
            }
        }
        else if ( bytes_read == 0 )
            break ;
        else
        {
            if ( errno == EAGAIN || errno == EWOULDBLOCK ) continue ;
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "recv: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: recv: " + std::string( std::strerror( errno ) ) );
        }
    }

    HttpData http = HttpRequests::parseRequest( full_request );
    Http::httpRequest( http, data, config );
    return ;
}

bool    Sockets::headersReceived( const std::string & request, int & content_length )
{
    size_t header_end_pos = request.find( "\r\n\r\n" );
    if ( header_end_pos != std::string::npos )
    {
        // Extract headers and find Content-Length
        std::string headers = request.substr( 0, header_end_pos + 4 );
        HttpData temp_http = HttpRequests::parseRequest( headers );

        if ( temp_http.headers.find( "Content-Length" ) != temp_http.headers.end() )
            content_length = ft::stoi( temp_http.headers[ "Content-Length" ].second );

        return ( true );
    }

    return ( false );
}

void    Sockets::sendConnection( Data & data )
{
    if ( send( data.conn_fd, data.response.c_str(), data.response.length(), 0 ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Sockets::closeConnection( int fd , std::string function, int line )
{
    if ( close( fd ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( function, line, "close: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: close: " + std::string( std::strerror( errno ) ) );
    }
    return ;
}