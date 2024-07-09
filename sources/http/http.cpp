/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:10:24 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/10 00:15:29 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

void    postRequest( HttpData & http, Data & data, ConfigData & config )
{
    UNUSED( config );
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "POST request" );

    std::string filepath = http.path.substr( http.path.find_last_of( "/" ) + 1 );

    // Extract boundary from Content-Type header
    std::string contentType = http.headers[ "Content-Type" ].second;
    std::string boundary = contentType.substr( contentType.find( "boundary=" ) + 9 );

    // Separate body parts using the boundary
    std::vector< std::string > parts = ft::split( http.body, "--" + boundary );
    for ( std::size_t i = 0; i < parts.size(); ++i )
    {
        std::string part = parts[ i ];

        // Skip empty parts or the last boundary part
        if ( part.empty() || part == "--\r\n" ) continue;

        // Split headers and content
        std::size_t headerEnd = part.find( "\r\n\r\n" );
        if ( headerEnd == std::string::npos ) continue;
        std::string headers = part.substr( 0, headerEnd );
        std::string content = part.substr( headerEnd + 4, part.size() - headerEnd - 6 );  // Remove trailing \r\n

        // Find filename in headers
        std::size_t filenamePos = headers.find( "filename=\"" );
        if ( filenamePos == std::string::npos ) continue;
        filenamePos += 10;  // Move past 'filename="'
        std::size_t filenameEnd = headers.find( "\"", filenamePos );
        if ( filenameEnd == std::string::npos ) continue;
        std::string filename = headers.substr( filenamePos, filenameEnd - filenamePos );

        // Construct file path
        std::string path_file = "./html/" + http.path.substr( http.path.find_last_of( "/" ) + 1 ) + "/" + filename;
        std::cout << "path_file: " << path_file << std::endl;
        // Save file content
        std::fstream *file = ft::openFile( path_file, std::ios::out | std::ios::binary );
        if ( file && file->is_open() )
        {
            file->write(content.c_str(), content.size());
            ft::closeFile(file);
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "File saved: " + filepath );
        }
        else
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to save file: " + filepath );
    }

    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
    data.response = response;
    return ;
}

void Http::httpRequest( HttpData & http, Data & data, ConfigData & config )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Request received" );

    std::string path = http.path == "/" ? "/index.html" : http.path;
    std::string fullPath = "./html" + path;

    struct stat info;
    std::string response;
    std::ostringstream response_stream;
    std::string response_body;

    switch ( http.method )
    {
        case ( GET ):
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "GET request" );

            if ( stat( fullPath.c_str(), &info ) != 0 )
                return ( HttpErrors::sendError( data, NOT_FOUND, config ) );            // Check if file exists
            else if ( info.st_mode & S_IFDIR )
                return ( Http::httpDirectoryListing( path, fullPath, data, config ) );  // Handle Directory listing
            else
                return ( Http::httpFileServing( path, fullPath, data, config ) );       // Handle file serving

            break ;
        case ( POST ):
            postRequest( http, data, config );
            break;
        default:
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Method not allowed" );
            HttpErrors::sendError( data, METHOD_NOT_ALLOWED, config );
            break ;
    }
}
