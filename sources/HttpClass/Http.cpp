/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:22:05 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 17:32:14 by cpeset-c         ###   ########.fr       */
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

void Http::handleRequest( const std::string & request )
{
    Request request_data = HttpRequestParser::deserializedRequest( request );

    // if ( request_data.method == GET )
    //     Http::handleGetRequest( request_data );
    // else if ( request_data.method == POST )
    //     Http::handlePostRequest( request_data );
    // else
    //     throw HttpMethodException( "Invalid HTTP method" );
}