/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 12:48:46 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/09 17:49:50 by cpeset-c         ###   ########.fr       */
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
    http.path = path;
    http.version = HTTP_VERSION;
    http.headers = HttpHeaders::deserializeHeader( request );
    http.body = "";
    
    // Find the end of headers
    size_t headerEndPos = buffer.find("\r\n\r\n");
    if (headerEndPos == std::string::npos)
    {
        // Handle case where headers are not properly terminated
        // You might want to throw an exception or handle this case as appropriate
        LOG(ERROR) << "Headers are not properly terminated in the request";
        return http;
    }

    std::string headers = buffer.substr(0, headerEndPos);
    http.headers = HttpHeaders::deserializeHeader(headers);

    // Check if Content-Length header is present to determine body length
    if (http.headers.find("Content-Length") != http.headers.end()) {
        int contentLength = ft::stoi(http.headers["Content-Length"].second);
        size_t bodyStartPos = headerEndPos + 4;

        // Ensure buffer has enough content to extract the body
        if (buffer.size() < bodyStartPos + contentLength) {
            // Handle case where the buffer does not contain enough data
            LOG(ERROR) << "Buffer does not contain enough data to extract body";
            LOG(ERROR) << "Buffer size: " << buffer.size() << ", Expected body size: " << bodyStartPos + contentLength;
            LOG(ERROR) << "Content-Length: " << contentLength;
            LOG(ERROR) << "Body start position: " << bodyStartPos;
            return http;
        }

        std::string body = buffer.substr(bodyStartPos, contentLength);
        http.body = body;

        LOG(DEBUG) << "Body:\n" << http.body;
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