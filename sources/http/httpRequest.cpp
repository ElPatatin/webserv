/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 12:48:46 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/04 16:11:12 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

HttpData    HttpRequests::parseRequest( std::string buffer )
{
    LOG( DEBUG ) << "Received:\n" << buffer;
    LOG( DEBUG ) << "Received size: " << buffer.size();
    
    std::string request( buffer );
    std::istringstream request_stream( request );
    std::string method, path;
    request_stream >> method >> path;   

    HttpData http;
    http.method = HttpMethods::methodFromString( method );
    http.path = path;
    http.version = HTTP_VERSION;
    http.headers = HttpHeaders::deserializeHeader( request );
    http.body = "";

    LOG( DEBUG ) << "Headers:\n" << HttpHeaders::serializeHeader( http.headers );

    return ( http );
}