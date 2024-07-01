/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:40:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/01 22:45:56 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

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

void    Sockets::bindSocket( Data *data, ConfigData config )
{
    data->addr_len = sizeof( data->addr );
    data->addr.sin_family = AF_INET;
    data->addr.sin_port = htons( config.getPort() );
    data->addr.sin_addr.s_addr = htonl( INADDR_ANY );
    std::memset( data->addr.sin_zero, '\0', sizeof( data->addr.sin_zero ) );

    if ( bind( data->conn_fd, reinterpret_cast< SockAddr * >( &data->addr ), static_cast< socklen_t >( data->addr_len ) ) == -1 )  
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "bind: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: bind: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Sockets::listenConnection( Data & data, int backlog )
{
    if ( listen( data.conn_fd, backlog ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "listen: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: listen: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Sockets::acceptConnection( Data *data )
{
    if ( ( data->new_fd = accept( data->conn_fd,\
            reinterpret_cast< SockAddr * >( &data->addr ),\
            reinterpret_cast< socklen_t * >( &data->addr_len ) ) ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "accept: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: accept: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Sockets::receiveConnection( Data *data )
{
    char    buffer[ 1024 ];

    std::memset( buffer, '\0', sizeof( buffer ) );
    if ( recv( data->new_fd, buffer, sizeof( buffer ), 0 ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "recv: " + std::string( std::strerror( errno ) ) );
        throw SocketException( "Error: recv: " + std::string( std::strerror( errno ) ) );
    }

    LOG( DEBUG ) << "Received: " << buffer;
    std::string request( buffer );
    std::istringstream request_stream( request );
    std::string method, path, protocol;
    request_stream >> method >> path >> protocol;

    if (method == "GET" && path == "/")
    {
        // Serve the index.html file
        std::fstream file;

        file.open( "./html/index.html", std::ios::in );
        
        if (!file.is_open())
        {
            std::fstream file_not_found; 
            file_not_found.open( "./html/errors/404.html", std::ios::in );
            std::string content( ( std::istreambuf_iterator<char>( file_not_found ) ), std::istreambuf_iterator<char>() );
            file_not_found.close();

            std::ostringstream response_stream;
            response_stream << "HTTP/1.1 404 Not Found\r\n"
                            << "Content-Length: " << content.length() << "\r\n"
                            << "Content-Type: text/html\r\n"
                            << "\r\n"
                            << content;
            
            std::string response = response_stream.str();
            if ( send( data->new_fd, response.c_str(), response.length(), 0 ) == -1 )
            {
                LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
                throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
            }
            
            return;
        }            

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.length() << "\r\n"
                        << "Content-Type: text/html\r\n"
                        << "\r\n"
                        << content;

        std::string response = response_stream.str();
        if ( send( data->new_fd, response.c_str(), response.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
    }
    else if ( method == "GET" && path == "/favicon.ico" )
    {
         // Serve the favicon.ico file
        std::ifstream file("./html/favicon.ico", std::ios::in | std::ios::binary);
        
        if (!file.is_open())
        {
            std::fstream file_not_found; 
            file_not_found.open( "./html/errors/404.html", std::ios::in );
            std::string content( ( std::istreambuf_iterator<char>( file_not_found ) ), std::istreambuf_iterator<char>() );
            file_not_found.close();

            std::ostringstream response_stream;
            response_stream << "HTTP/1.1 404 Not Found\r\n"
                            << "Content-Length: " << content.length() << "\r\n"
                            << "Content-Type: text/html\r\n"
                            << "\r\n"
                            << content;
            
            std::string response = response_stream.str();
            if ( send( data->new_fd, response.c_str(), response.length(), 0 ) == -1 )
            {
                LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
                throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
            }
            
            return;
        }

        std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.size() << "\r\n"
                        << "Content-Type: image/x-icon\r\n"
                        << "\r\n";

        std::string header = response_stream.str();
        if ( send( data->new_fd, header.c_str(), header.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
        if ( send( data->new_fd, content.data(), content.size(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
    }
    else if ( method == "GET" )
    {
        std::fstream file_not_found; 
        file_not_found.open( "./html/errors/404.html", std::ios::in );
        std::string content( ( std::istreambuf_iterator<char>( file_not_found ) ), std::istreambuf_iterator<char>() );
        file_not_found.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 404 Not Found\r\n"
                        << "Content-Length: " << content.length() << "\r\n"
                        << "Content-Type: text/html\r\n"
                        << "\r\n"
                        << content;
        
        std::string response = response_stream.str();
        if ( send( data->new_fd, response.c_str(), response.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
        
        return;
    }
    else
    {
        // Method not supported or not found
        std::string response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
        if ( send( data->new_fd, response.c_str(), response.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
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