/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpErrors.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:11:20 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/04 12:19:14 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

void    HttpErrors::sendError( Data & data, int status_code, ConfigData config )
{
    UNUSED( config );
    UNUSED( status_code );

    std::fstream file_not_found; 
    file_not_found.open( "./html/errors/404.html", std::ios::in );
    std::string content( ( std::istreambuf_iterator<char>( file_not_found ) ), std::istreambuf_iterator<char>() );
    file_not_found.close();

    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 404 Not Found\r\n"
                    << "Content-Length: " << content.length() << "\r\n"
                    << "Content-Type: text/html\r\n"
                    << "\r\n"
                    << content;

    std::string response = response_stream.str();
    if ( send( data.new_fd, response.c_str(), response.length(), 0 ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: error" );
        throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}