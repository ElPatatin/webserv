/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpSaveFile.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 12:08:41 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 13:35:14 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpFileServing.hpp"

void HttpFileServing::httpSaveFile( Data & data, const HttpRequestParser::Request & request, const std::string & full_path, const std::string & root )
{
    UNUSED( full_path );
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "POST request" );

    std::string filepath = request.url.substr( request.url.find_last_of( "/" ) + 1 );

    // Extract boundary from Content-Type header
    std::string contentType = const_cast< HttpRequestParser::Request & >( request ).headers[ "Content-Type" ].second;
    std::string boundary = contentType.substr( contentType.find( "boundary=" ) + 9 );

    // Separate body parts using the boundary
    std::vector< std::string > parts = ft::split( request.body, "--" + boundary );
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
        std::string path_file = root + "/" + request.url.substr( request.url.find_last_of( "/" ) + 1 ) + "/" + filename;
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

    HttpFileServing::httpDataServing( data, request, 200, "File saved: " + filepath );
    return ;
}