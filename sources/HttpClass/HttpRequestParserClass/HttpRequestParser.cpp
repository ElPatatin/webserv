/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:26:49 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 12:30:48 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser( void ) { return ; }

HttpRequestParser::HttpRequestParser( const HttpRequestParser & src ) { *this = src; return ; }

HttpRequestParser & HttpRequestParser::operator=( const HttpRequestParser & rhs )
{
    if ( this != &rhs )
    {
        *this = rhs;
    }
    return *this;
}

HttpRequestParser::~HttpRequestParser( void ) { return ; }

HttpRequestParser::Request HttpRequestParser::parseRequest( const std::string & request )
{
    Request request_data;

    std::istringstream request_stream( request );
    std::string method, url, version, line;
    request_stream >> method >> url >> version;
    
    request_data.method = HttpMethods::fromString( method );
    request_data.version = HttpVersion::fromString( version );
    HttpRequestParser::parseUrl( request_data, url );
}

void    HttpRequestParser::parseUrl( Request & request, const std::string & line )
{
    std::string url = HttpUrl::decode( line );
    std::vector< std::string > url_parts = ft::split( url, "?" );
    if ( url_parts.size() == 2 )
    {
        request.url = url_parts[ 0 ];
        request.query = HttpRequestParser::parseQuery( url_parts[ 1 ] );
    }
    else if ( url_parts.size() == 1 )
    {
        request.url = line;
        request.query = "";
    }
    else
        throw HttpUrlException( "Invalid URL" );

    return ;
}

// Parse query. If it found same key, it will be removed the last key from the query
std::string HttpRequestParser::parseQuery( const std::string & query )
{
    
    return ( query );
}