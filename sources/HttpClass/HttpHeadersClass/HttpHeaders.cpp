/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:01:04 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 11:34:24 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpHeaders.hpp"

// Default constructor
HttpHeaders::HttpHeaders( void ) { return ; }

// Copy constructor
HttpHeaders::HttpHeaders( const HttpHeaders & src ) { *this = src; return ; }

// Assignment operator
HttpHeaders& HttpHeaders::operator=( const HttpHeaders & rhs )
{
    if (this != &rhs)
    {
        // No member variables to assign
    }

    return ( *this );
}

// Destructor
HttpHeaders::~HttpHeaders( void ) { return ; }

// Serialize headers to string
std::string HttpHeaders::serializeHeader( const Headers & headers )
{
    std::ostringstream header_stream;

    for ( Headers::const_iterator it = headers.begin(); it != headers.end(); ++it )
    {
        const Header & header = it->second;
        header_stream << header.first << ": " << header.second << LINE_END;
    }

    return ( header_stream.str() );
}

// Deserialize headers from string
Headers HttpHeaders::deserializeHeader( const std::string & header )
{
    Headers headers;
    Header  header_pair;

    std::istringstream header_stream( header );
    std::string line;
    while ( std::getline(header_stream, line ) )
    {
        // Skip first line
        if ( line.find( "HTTP" ) != std::string::npos )
            continue ;

        size_t pos = line.find( ": " );
        if ( pos == std::string::npos )
            break ;

        header_pair.first = line.substr( 0, pos );
        header_pair.second = line.substr( pos + 2 );
        headers.insert( std::make_pair( header_pair.first, header_pair ) );
    }

    return ( headers );
}
