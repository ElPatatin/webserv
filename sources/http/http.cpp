/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:10:24 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/07 21:26:33 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sys/stat.h> // For file info

void Http::httpRequest( HttpData &http, Data &data, ConfigData & config )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Request received" );

    std::string path = http.path == "/" ? "/index.html" : http.path;
    std::string fullPath = "./html" + path;

    struct stat info;
    std::string response;
    std::fstream *file;
    std::ostringstream response_stream;
    std::string response_body;
    size_t content_length;


    switch ( http.method )
    {
        case ( GET ):
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "GET request" );

            if ( stat( fullPath.c_str(), &info ) != 0 )
                return ( HttpErrors::sendError( data, NOT_FOUND, config ) );              // Check if file exists
            else if ( info.st_mode & S_IFDIR )
                return ( Http::httpDirectoryListing( path, fullPath, data, config ) );  // Handle Directory listing
            else
                return ( Http::httpFileServing( path, fullPath, data, config ) );       // Handle file serving

            break;
        case ( POST ):
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "POST request" );

            file = ft::openFile( fullPath.c_str(), std::ios::out );
            if (!file) {
                LOG(ERROR) << ft::prettyPrint(__FUNCTION__, __LINE__, "Failed to open file for writing");
                return HttpErrors::sendError(data, INTERNAL_SERVER_ERROR, config);
            }
            *file << http.body;
            ft::closeFile( file );

            // Create a response body
            response_body = "<html><body><h1>File saved</h1></body></html>";
            content_length = response_body.size();

            // Send a response back to the client
            response_stream << "HTTP/1.1 200 OK\r\n"
                            << "Content-Length: " << content_length << "\r\n"
                            << "Content-Type: text/html\r\n"
                            << "\r\n"
                            << response_body;

            response = response_stream.str();
            if ( send( data.new_fd, response.c_str(), response.length(), 0 ) == -1 )
            {
                LOG( ERROR ) << ft::prettyPrint(__FUNCTION__, __LINE__, "send: " + std::string(std::strerror(errno)));
                throw SocketException("Error: send: " + std::string(std::strerror(errno)));
            }

            break;
        default:
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Method not allowed" );
            HttpErrors::sendError( data, METHOD_NOT_ALLOWED, config );
            break;
    }
}
