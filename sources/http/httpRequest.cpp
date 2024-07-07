/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 12:48:46 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/07 20:55:21 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

HttpData    HttpRequests::parseRequest( std::string buffer )
{
    LOG( INFO ) << "Received size: " << buffer.size();
    LOG( INFO ) << "Received:\n" << buffer;
    
    std::string request( buffer );
    std::istringstream request_stream( request );
    std::string method, path;
    request_stream >> method >> path;   

    HttpData http;
    http.method = HttpMethods::methodFromString( method );
    http.path = path;
    http.version = HTTP_VERSION;
    http.headers = HttpHeaders::deserializeHeader( request );

    // Look for Content-Length header to determine body length
    Headers::const_iterator it = http.headers.find("Content-Length");
    if ( it != http.headers.end() )
    {
        int contentLength = std::atoi( it->second.second.c_str() );
        size_t headerEndPos = buffer.find("\r\n\r\n");
        if ( headerEndPos != std::string::npos )
        {
            size_t bodyStartPos = headerEndPos + 4;
            http.body = buffer.substr(bodyStartPos, contentLength);
        }
    }
    else
        http.body = "";

    return ( http );
}