/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersion.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:02:55 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 15:59:45 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpVersion.hpp"

// Default constructor
HttpVersion::HttpVersion( void ) { return ; }

// Copy constructor
HttpVersion::HttpVersion( const HttpVersion & src ) { return ; }

// Assignment operator
HttpVersion &   HttpVersion::operator=( const HttpVersion & rhs )
{
    if ( this != &rhs )
    {
        
    }
    return ( *this );
}

// Destructor
HttpVersion::~HttpVersion( void ) { return ; }

std::string HttpVersion::toString( const size_t & hash )
{
    switch ( hash )
    {
        case HTTP_1_0: return ( HTTP_VERSION_1_0 );
        case HTTP_1_1: return ( HTTP_VERSION_1_1 );
        case HTTP_2_0: return ( HTTP_VERSION_2_0 );
        default: return ( NULL );
    }
}

HttpVersion::Version HttpVersion::fromString( const std::string & version )
{

    switch ( hash( version ) )
    {
        case HTTP_1_0_HASH: return ( HTTP_1_0 );
        case HTTP_1_1_HASH: return ( HTTP_1_1 );
        case HTTP_2_0_HASH: return ( HTTP_2_0 );
        default: return ( HTTP_1_1 );
    }
}

unsigned int    HttpVersion::hash( const std::string & str )
{
    unsigned int hash = 0;
    for ( size_t i = 0; i < str.size(); ++i )
        hash = hash * 31 + str[ i ];

    return ( hash );
}
