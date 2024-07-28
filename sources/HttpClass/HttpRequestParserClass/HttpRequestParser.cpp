/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:26:49 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/28 19:49:47 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser( void ) { return ; }

HttpRequestParser::HttpRequestParser( const HttpRequestParser & src ) : HttpHeaders( src ), HttpMethods( src ), HttpResponse( src ), HttpUrl( src ), HttpVersion( src ) { return ; }

HttpRequestParser & HttpRequestParser::operator=( const HttpRequestParser & rhs )
{
    if ( this != &rhs )
    {
        *this = rhs;
    }
    return *this;
}

HttpRequestParser::~HttpRequestParser( void ) { return ; }

HttpRequestParser::Request HttpRequestParser::deserializedRequest( const std::string & request )
{
    Request request_data;

    std::istringstream request_stream( request );
    std::string method, url, version, line;
    request_stream >> method >> url >> version;

    try { request_data.method = HttpMethods::fromString( method ); }
    catch ( const std::exception & e ) { LOG( ERROR ) << e.what(); }

    request_data.version = HttpVersion::fromString( version );
    // if ( request_data.version != HTTP_1_1 )
    //     throw HttpVersionException( "Invalid HTTP version" );
    HttpRequestParser::parseUrl( request_data, url );
    request_data.isCGI = HttpRequestParser::parseIsCGI( request_data.url );

    size_t headerEndPos = request.find("\r\n\r\n");
    HttpRequestParser::parseHeaders( request_data, request, headerEndPos );
    HttpRequestParser::parseBody( request_data, request, headerEndPos );

    return ( request_data );
}

std::string HttpRequestParser::serializeRequest( const Request & request )
{
    std::string result = HttpMethods::toString( request.method ) + " " + request.url + " " + HttpVersion::toString( request.version ) + "\r\n";

    result += HttpHeaders::serializeHeader( request.headers );

    result += "\r\n" + request.body;

    return ( result );
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

void    HttpRequestParser::parseHeaders( Request & request_data, const std::string & request, size_t headerEndPos )
{
    if ( headerEndPos == std::string::npos )
    {
        LOG( ERROR ) << "Headers are not properly terminated in the request";
        throw HttpHeadersException( "Headers are not properly terminated in the request" );
    }

    std::string headers = request.substr(0, headerEndPos);
    request_data.headers = HttpHeaders::deserializeHeader( headers );

    return ;
}

void    HttpRequestParser::parseBody( Request & request_data, const std::string & request, size_t headerEndPos )
{
    // Check if Content-Length header is present to determine body length
    if ( request_data.headers.find("Content-Length") != request_data.headers.end() )
    {
        int contentLength = ft::stoi( request_data.headers["Content-Length"].second );
        size_t bodyStartPos = headerEndPos + 4;

        if ( request.size() < bodyStartPos + contentLength )
        {
            LOG( ERROR ) << "Invalid body";
            throw HttpBodyException( "Invalid body" );
        }

        std::string body = request.substr( bodyStartPos, contentLength );
        request_data.body = body;
    }
    else
        request_data.body = "";

    return ;
}

void    HttpRequestParser::parseCookie( Request & request_data )
{
    UNUSED( request_data );

    return ;
}
