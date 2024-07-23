/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:26:49 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 16:39:27 by cpeset-c         ###   ########.fr       */
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
    if ( request_data.version != HTTP_1_1 )
        throw HttpVersionException( "Invalid HTTP version" );
    HttpRequestParser::parseUrl( request_data, url );
    request_data.isCGI = HttpRequestParser::parseIsCGI( request_data.url );
}

void    HttpRequestParser::parseUrl( Request & request, const std::string & line )
{
    std::vector< std::string > url_parts = ft::split( line, "?" );
    if ( url_parts.size() == 2 )
    {
        request.url = HttpUrl::decode( url_parts[ 0 ] );
        request.query = HttpUrl::decode( HttpRequestParser::parseQuery( url_parts[ 1 ] ) );
    }
    else if ( url_parts.size() == 1 )
    {
        request.url = HttpUrl::decode( line );;
        request.query = "";
    }
    else
        throw HttpUrlException( "Invalid URL" );

    return ;
}

std::string HttpRequestParser::parseQuery( const std::string & query )
{
    std::vector< std::string > query_parts = ft::split( query, "&" );

    std::map< std::string, std::string > query_map;
    for ( size_t i = 0; i < query_parts.size(); ++i )
    {
        std::vector< std::string > query_key_value = ft::split( query_parts[ i ], "=" );
        if ( query_key_value.size() != 2 )
            throw HttpUrlException( "Invalid query" );
        if ( query_map.find( query_key_value[ 0 ] ) != query_map.end() )
            continue ;
        query_map[ query_key_value[ 0 ] ] = query_key_value[ 1 ];
    }

    std::string result = "";
    for ( std::map< std::string, std::string >::iterator it = query_map.begin(); it != query_map.end(); ++it )
    {
        if ( it == query_map.begin() )
            result += it->first + "=" + it->second;
        else
            result += "&" + it->first + "=" + it->second;
    }

    return ( result );
}

bool    HttpRequestParser::parseIsCGI( const std::string & url )
{
    if ( url.find( ".cgi" ) != std::string::npos || url.find( "/cgi-bin/" ) != std::string::npos )
        return ( true );
    return ( false );
}