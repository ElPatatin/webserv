/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:10:24 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/05 14:57:04 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

void    Http::httpRequest( HttpData & http, Data & data, ConfigData config )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Request received" );
    if ( HttpMethods::toString( http.method ) == "GET" && ( http.path == "/" || http.path == "/index.html" ) )
    {
        LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "GET /" );
        // Serve the index.html fileresponse_stream
        std::fstream file;

        file.open( "./html/index.html", std::ios::in );
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

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
    else if ( HttpMethods::toString( http.method ) == "GET" && http.path == "/favicon.ico" )
    {
         // Serve the favicon.ico file
        std::ifstream file("./html/favicon.ico", std::ios::in | std::ios::binary);
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

        std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.size() << "\r\n"
                        << "Content-Type: image/x-icon\r\n"
                        << "\r\n";

        std::string header = response_stream.str();
        if ( send( data.new_fd, header.c_str(), header.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
        if ( send( data.new_fd, content.data(), content.size(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
    }
    else if ( HttpMethods::toString( http.method ) == "GET" && http.path == "/resources/about-hero.jpg" )
    {
        // Serve the cpeset-c.jpg file
        std::ifstream file("./html/resources/about-hero.jpg", std::ios::in | std::ios::binary);
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

        std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.size() << "\r\n"
                        << "Content-Type: image/jpeg\r\n"
                        << "\r\n";

        std::string header = response_stream.str();
        if ( send( data.new_fd, header.c_str(), header.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
        if ( send( data.new_fd, content.data(), content.size(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
    }
    else if ( HttpMethods::toString( http.method ) == "GET" && http.path == "/resources/contact-hero.jpg" )
    {
        // Serve the cpeset-c.jpg file
        std::ifstream file("./html/resources/contact-hero.jpg", std::ios::in | std::ios::binary);
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

        std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.size() << "\r\n"
                        << "Content-Type: image/jpeg\r\n"
                        << "\r\n";

        std::string header = response_stream.str();
        if ( send( data.new_fd, header.c_str(), header.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
        if ( send( data.new_fd, content.data(), content.size(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
    }
    else if ( HttpMethods::toString( http.method ) == "GET" && http.path == "/resources/error-hero.jpg" )
    {
        // Serve the cpeset-c.jpg file
        std::ifstream file("./html/resources/error-hero.jpg", std::ios::in | std::ios::binary);
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

        std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.size() << "\r\n"
                        << "Content-Type: image/jpeg\r\n"
                        << "\r\n";

        std::string header = response_stream.str();
        if ( send( data.new_fd, header.c_str(), header.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
        if ( send( data.new_fd, content.data(), content.size(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
    }
    else if ( HttpMethods::toString( http.method ) == "GET" && http.path == "/resources/main-hero.jpg" )
    {
        // Serve the cpeset-c.jpg file
        std::ifstream file("./html/resources/main-hero.jpg", std::ios::in | std::ios::binary);
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

        std::vector<char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << content.size() << "\r\n"
                        << "Content-Type: image/jpeg\r\n"
                        << "\r\n";

        std::string header = response_stream.str();
        if ( send( data.new_fd, header.c_str(), header.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
        if ( send( data.new_fd, content.data(), content.size(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
    }
    else if ( HttpMethods::toString( http.method ) == "GET" && http.path == "/about.html" )
    {
        // Serve the about.html file
        std::ifstream file("./html/about.html", std::ios::in);
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

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
    else if ( HttpMethods::toString( http.method ) == "GET" && http.path == "/contact.html" )
    {
        // Serve the contact.html file
        std::ifstream file("./html/contact.html", std::ios::in);
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

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
    else if ( HttpMethods::toString( http.method ) == "GET" && http.path == "/errors.html" )
    {
        // Serve the errors.html file
        std::ifstream file("./html/errors.html", std::ios::in);
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

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
    else if ( HttpMethods::toString( http.method ) == "GET" && http.path == "/errors/404.html" )
    {
        // Serve the 404.html file
        std::ifstream file("./html/errors/404.html", std::ios::in);
        
        if (!file.is_open())
            return ( HttpErrors::sendError( data, 404, config ), void() );

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
    else if ( HttpMethods::toString( http.method ) == "GET" )
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
        if ( send( data.new_fd, response.c_str(), response.length(), 0 ) == -1 )
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
        if ( send( data.new_fd, response.c_str(), response.length(), 0 ) == -1 )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
        }
    }
    return ;
}