/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 12:48:46 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/02 11:12:01 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

void    Http::httpRequest( HttpData & http, Data & data, ConfigData config )
{
    if ( http.method == "GET" && http.path == "/")
    {
        // Serve the index.html file
        std::fstream file;

        file.open( "./html/index.html", std::ios::in );
        
        if (!file.is_open())
            Http::sendError( data, 404, config );

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.length() << "\r\n"
                        << "Content-Type: text/html\r\n"
                        << "\r\n"
                        << content;

        std::string response = response_stream.str();
        if ( send( data.new_fd, response.c_str(), response.length(), 0 ) == -1 )
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

void    Http::sendError( Data & data, int status_code, ConfigData config )
{
    UNUSED( config );
    UNUSED( status_code );
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