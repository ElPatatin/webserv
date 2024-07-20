/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeaders.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:13:42 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/04 16:06:26 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

std::string HttpHeaders::serializeHeader( Headers headers )
{
    std::ostringstream header_stream;

    Header header;
    for ( Headers::iterator it = headers.begin(); it != headers.end(); it++ )
    {
        header = it->second;
        header_stream << header.first << ": " << header.second << LINE_END;
    }

    return ( header_stream.str() );
}

Headers HttpHeaders::deserializeHeader( std::string header )
{
    Headers headers;
    Header  header_pair;

    std::istringstream header_stream( header );
    std::string line;
    while ( std::getline( header_stream, line ) )
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