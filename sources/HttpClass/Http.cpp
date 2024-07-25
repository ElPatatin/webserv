/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:22:05 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/25 11:36:14 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http( void ) { return ; }

Http::Http( const Http & src ) { *this = src; return ; }

Http & Http::operator=( const Http & rhs )
{
    if ( this != &rhs )
    {
        *this = rhs;
    }
    return *this;
}

Http::~Http( void ) { return ; }

void Http::handleRequest( const std::string & request, const ConfigData & config_data, const Data & data )
{
    Request request_data = HttpRequestParser::deserializedRequest( request );
    std::string full_url = Http::getFullUrl( config_data.getRoot(), request_data.url );
    
    switch ( request_data.method )
    {
        case GET:
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "GET request" );
            break;
        case POST:
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "POST request" );
            break;
        case PUT:
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "PUT request" );
            break;
        case DELETE:
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "DELETE request" );
            break;
        case HEAD:
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "HEAD request" );
            break;
        default:
            throw std::runtime_error( "Error: invalid request" );
    }
}

std::string Http::getFullUrl( const std::string & root, const std::string & url )
{
    if ( root.empty() || access( root.c_str(), F_OK ) == -1 )
        throw std::runtime_error( "Error: root path is not valid" );

    std::string full_url = root + url;
    return ( full_url);
}