/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:10:24 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 12:15:22 by cpeset-c         ###   ########.fr       */
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
        filenamePos += 10;
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

void    redirection( Data & data, ConfigData & config, std::string path )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Redirecting to: " + path );

    UNUSED( config );
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Path: " + path );
    if ( path == "/redirection" )
        path = "https://www.youtube.com/watch?v=OgIRAjnnJzI";
    else if ( path == "/tig" )
        path = "https://cdn.intra.42.fr/users/f501b2a1aa0ff80e6a5f5797a77946bf/jagarcia.jpg";

    std::string response = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + path + "\r\nContent-Length: 0\r\n\r\n";
    data.response = response;
}

void Http::httpRequest( HttpData & http, Data & data, ConfigData & config )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Request received" );

    std::string path = Url::decode( http.path );
    std::string fullPath = "./html" + path;

    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Path: " + path );
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Full path: " + fullPath );
    struct stat info;
    std::string response;
    std::ostringstream response_stream;
    std::string response_body;

    switch ( http.method )
    {
        case ( GET ):
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "GET request" );

            // Check if the path is a redirection
            LOG( DEBUG ) << path.find( "redirection" );
            if ( path.find( "redirection" ) != std::string::npos || path.find( "tig" ) != std::string::npos )
                return ( redirection( data, config, path ) );

            // Check if file exists
            if ( stat( fullPath.c_str(), &info ) != 0 )
                return ( HttpErrors::sendError( data, NOT_FOUND, config ) );
            
            if ( http.isCGI )
                return ( Http::executeCGI( fullPath, http.params, response_stream, data, config, http ) );

            if ( info.st_mode & S_IFDIR )           // Handle Directory listing
                return ( Http::httpDirectoryListing( path, fullPath, data, config ) );
            else if ( info.st_mode & S_IFREG )      // Handle file serving
                return ( Http::httpFileServing1( path, fullPath, data, config ) );
            else                                     // Internal server error
                return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

            break ;
        case ( POST ):
            postRequest( http, data, config );
            break ;
        case ( DELETE ):
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "DELETE request" );

            if ( stat( fullPath.c_str(), &info ) != 0 )
                return ( HttpErrors::sendError( data, NOT_FOUND, config ) );
            
            if ( std::remove( fullPath.c_str() ) != 0 )
                return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

            response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";

            data.response = response;
            break ;
        default:
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Method not allowed" );
            HttpErrors::sendError( data, METHOD_NOT_ALLOWED, config );
            break ;
    }
}
