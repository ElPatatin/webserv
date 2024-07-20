/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersion.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:02:55 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/20 15:38:49 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpVersion.hpp"

// Default constructor
HttpVersion::HttpVersion( void )
    : _type( HTTP_1_1 ) { return ; }

// Parameterized constructor
HttpVersion::HttpVersion( Type type )
    : _type( type ) { return ; }

// Parameterized constructor
HttpVersion::HttpVersion( const std::string & version )
    : _type( fromString( version ) ) { return ; }

// Copy constructor
HttpVersion::HttpVersion( const HttpVersion & src )
    : _type( src._type ) { return ; }

// Assignment operator
HttpVersion &   HttpVersion::operator=( const HttpVersion & rhs )
{
    if ( this != &rhs )
    {
        _type = rhs._type;
    }
    return ( *this );
}

// Destructor
HttpVersion::~HttpVersion( void ) { return ; }

std::string HttpVersion::toString( void ) const
{
    switch ( _type )
    {
        case HTTP_1_0: return ( HTTP_VERSION_1_0 );
        case HTTP_1_1: return ( HTTP_VERSION_1_1 );
        case HTTP_2_0: return ( HTTP_VERSION_2_0 );
        default: return ( NULL );
    }
}

HttpVersion::Type HttpVersion::fromString( const std::string & version )
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
