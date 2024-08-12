/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpUrl.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:02:59 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/20 15:51:27 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpUrl.hpp"

HttpUrl::HttpUrl( void ) { return; }

HttpUrl::HttpUrl( const HttpUrl & src ) { *this = src; return ; }

HttpUrl &   HttpUrl::operator=( const HttpUrl & rhs )
{
    if ( this != &rhs )
    {
        // No member variables to assign
    }
    return ( *this );
}

HttpUrl::~HttpUrl( void ) { return ; }

std::string HttpUrl::decode( const std::string & url )
{
    std::ostringstream  unescaped;

    for ( std::string::const_iterator i = url.begin(), n = url.end(); i != n; ++i )
    {
        std::string::value_type c = ( *i );

        if ( c == '%' )
        {
            if ( std::distance( i, url.end() ) >= 3 && isxdigit( *( i + 1 ) ) && isxdigit( *( i + 2 ) ) )
            {
                std::string hex = std::string( i + 1, i + 3 );
                std::istringstream iss( hex );
                int value;
                iss >> std::hex >> value;

                unescaped << static_cast< char >( value );
                i += 2;
            }
            else
                unescaped << c;
        }
        else if ( c == '+' )
            unescaped << ' ';
        else
            unescaped << c;
    }

    return ( unescaped.str() );
}

std::string HttpUrl::encode(const std::string& url)
{
    std::ostringstream escaped;
    escaped.fill( '0' );
    escaped << std::hex;

    for ( std::string::const_iterator i = url.begin(), n = url.end(); i != n; ++i )
    {
        std::string::value_type c = ( *i );

        // Keep alphanumeric and other accepted characters intact
        if ( std::isalnum( c ) || c == '-' || c == '_' || c == '.' || c == '~' )
        {
            escaped << c;
            continue ;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw( 2 ) << int( ( unsigned char ) c );
        escaped << std::nouppercase;
    }

    return ( escaped.str() );
}
