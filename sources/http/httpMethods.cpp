/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpMethods.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:07:41 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/12 14:58:33 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

std::string HttpMethods::toString( Method method )
{
    switch ( method )
    {
        case GET: return ( "GET" );
        case HEAD: return ( "HEAD" );
        case POST: return ( "POST" );
        case PUT: return ( "PUT" );
        case DELETE: return ( "DELETE" );
        case TRACE: return ( "TRACE" );
        case OPTIONS: return ( "OPTIONS" );
        case CONNECT: return ( "CONNECT" );
        case PATCH: return ( "PATCH" );
        default: throw MethodNotAllowedException( "Method to string not allowed" );
    }
}

Method HttpMethods::methodFromString( const std::string & method )
{
    static const std::pair<std::string, Method> method_pairs[] = {
        std::make_pair( "GET", GET ),
        std::make_pair( "HEAD", HEAD ),
        std::make_pair( "POST", POST ),
        std::make_pair( "PUT", PUT ),
        std::make_pair( "DELETE", DELETE ),
        std::make_pair( "TRACE", TRACE ),
        std::make_pair( "OPTIONS", OPTIONS ),
        std::make_pair( "CONNECT", CONNECT ),
        std::make_pair( "PATCH", PATCH )
    };

    static const std::map< std::string, Method > method_map(
        method_pairs, method_pairs + sizeof( method_pairs ) / sizeof( method_pairs[ 0 ] )
    );

    std::map< std::string, Method >::const_iterator it = method_map.find( method );
    if ( it != method_map.end() )
        return ( it->second );

    LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Method not allowed" );

    throw MethodNotAllowedException( "String to method not allowed" );
}