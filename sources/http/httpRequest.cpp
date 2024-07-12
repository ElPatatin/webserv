/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 12:48:46 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/11 12:57:15 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

HttpData    HttpRequests::parseRequest( std::string buffer )
{
    HttpData http;

    std::string request( buffer );
    std::istringstream request_stream( request );
    std::string method, path;
    request_stream >> method >> path;

    http.method = HttpMethods::methodFromString( method );
    http.version = HTTP_VERSION;
    http.headers = HttpHeaders::deserializeHeader( request );
    http.body = "";

    std::vector< std::string > path_parts = ft::split( path, "?" );
    if ( path_parts.size() > 1 )
    {
        http.path = path_parts[ 0 ];
        http.params = path_parts[ 1 ];
    }
    else
    {
        http.path = path;
        http.params = "";
    }

    // Identify if the request is for a CGI script
    if ( http.path.find( ".cgi" ) != std::string::npos || http.path.find( "/cgi-bin/" ) != std::string::npos )
        http.isCGI = true;
    else
        http.isCGI = false;
    
    // Find the end of headers
    size_t headerEndPos = buffer.find("\r\n\r\n");
    if (headerEndPos == std::string::npos)
    {
        LOG(ERROR) << "Headers are not properly terminated in the request";
        return http;
    }

    std::string headers = buffer.substr(0, headerEndPos);
    http.headers = HttpHeaders::deserializeHeader(headers);

    // Check if Content-Length header is present to determine body length
    if (http.headers.find("Content-Length") != http.headers.end())
    {
        int contentLength = ft::stoi(http.headers["Content-Length"].second);
        size_t bodyStartPos = headerEndPos + 4;

        if (buffer.size() < bodyStartPos + contentLength)
        {
            LOG(ERROR) << "Body length is less than the content length specified in the header";
            LOG(ERROR) << "Body length: " << buffer.size() - bodyStartPos;
            LOG(ERROR) << "Content-Length: " << contentLength;
            return ( http );
        }

        std::string body = buffer.substr(bodyStartPos, contentLength);
        http.body = body;
    }

    std::ostringstream response_stream;
    response_stream << "Method: " << HttpMethods::toString( http.method ) << std::endl;
    response_stream << "Path: " << http.path << std::endl;
    response_stream << "Version: " << http.version << std::endl;
    for ( Headers::iterator it = http.headers.begin(); it != http.headers.end(); it++ )
        response_stream << it->first << ": " << it->second.first << " - " << it->second.second << std::endl;
    LOG( DEBUG ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Request received: \n" + response_stream.str() );

    return ( http );
}