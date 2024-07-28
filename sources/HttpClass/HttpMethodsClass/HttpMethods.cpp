/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethods.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:01:29 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/28 15:34:22 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMethods.hpp"

HttpMethods::Methods HttpMethods::methodMap = HttpMethods::createMethodMap();

HttpMethods::HttpMethods( void ) { return ; }

HttpMethods::HttpMethods( const HttpMethods & src ) { *this = src; return ; }

HttpMethods& HttpMethods::operator=( const HttpMethods & rhs )
{
    if ( this != &rhs )
    {
        // No member variables to assign
    }
    return ( *this );
}

HttpMethods::~HttpMethods( void ) { return ; }

std::string HttpMethods::toString( Method method )
{
    switch ( method )
    {
        case GET: return ( "GET" );
        case HEAD: return ( "HEAD" );
        case POST: return ( "POST ");
        case PUT: return ( "PUT ");
        case DELETE: return ( "DELETE ");
        case TRACE: return ( "TRACE ");
        case OPTIONS: return ( "OPTIONS ");
        case CONNECT: return ( "CONNECT ");
        case PATCH: return ( "PATCH ");
        default: throw MethodNotAllowedException( "Method: method to string not allowed" );
    }
}

HttpMethods::Method HttpMethods::fromString( const std::string & method )
{
    HttpMethods::Methods::const_iterator it = methodMap.find( method );
    if ( it != methodMap.end() )
        return ( it->second );

    LOG( ERROR ) << "Method: " << method << " not allowed" << std::endl;
    throw MethodNotAllowedException( "Method: string to method not allowed" );
}

std::bitset<9> HttpMethods::getMethodBitMap( const std::string & allow_methods )
{
    std::bitset<9> bit_map = 0b000000000;

    std::vector< std::string > methods = ft::split( allow_methods, "," );
    for ( size_t i = 0; i < methods.size(); ++i )
    {
        try
        {
            Method method = fromString( methods[ i ] );
            bit_map.set( method );
        }
        catch ( const MethodNotAllowedException & e ) { return ( 0b000000000 ); }
    }

    return ( bit_map );
}

HttpMethods::Methods HttpMethods::createMethodMap( void )
{
    HttpMethods::Methods map;

    map[ "GET" ] = GET;
    map[ "HEAD" ] = HEAD;
    map[ "POST" ] = POST;
    map[ "PUT" ] = PUT;
    map[ "DELETE" ] = DELETE;
    map[ "TRACE" ] = TRACE;
    map[ "OPTIONS" ] = OPTIONS;
    map[ "CONNECT" ] = CONNECT;
    map[ "PATCH" ] = PATCH;

    return ( map );
}
