/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeaders.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:13:42 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/04 13:14:27 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

std::string HttpHeaders::serializeHeader( std::string key, std::string value )
{
    return ( key + ": " + value + LINE_END );
}

Headers HttpHeaders::deserializeHeader( std::string header )
{
    Headers headers;
    std::vector<std::string> header_lines = ft::split( header, LINE_END );

    // for ( std::vector<std::string>::iterator it = header_lines.begin(); it != header_lines.end(); it++ )
    // {
    //     std::vector<std::string> header = ft::split( *it, ": " );
    //     if ( header.size() == 2 )
    //         headers[header[0]] = header[1];
    // }

    return ( headers );
}