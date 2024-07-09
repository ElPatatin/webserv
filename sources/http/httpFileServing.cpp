/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpFileServing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:34:05 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/09 16:09:39 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

void    Http::httpFileServing( std::string path, std::string fullPath, Data &data, ConfigData &config )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Serving file" );

    std::string filename = path.substr( path.find_last_of( "/" ) + 1 );
    std::string::size_type dotPos = filename.find_last_of( "." );
    if ( dotPos != std::string::npos )
    {
        std::string statusCodeStr = filename.substr( 0, dotPos );
        int status_code;
        std::istringstream( statusCodeStr ) >> status_code;
        
        if ( status_code )
            return ( HttpErrors::sendError( data, status_code, config ) );
    }


    std::ifstream file( fullPath.c_str(), std::ios::in | 
        ( fullPath.find( ".jpg" ) != std::string::npos || fullPath.find( ".ico" ) != std::string::npos ? std::ios::binary : std::ios::in ) );

    if ( !file.is_open( ) )
        return HttpErrors::sendError( data, NOT_FOUND, config );

    std::string content( ( std::istreambuf_iterator< char >( file ) ), std::istreambuf_iterator< char >() );
    file.close();

    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 200 OK\r\n"
                    << "Content-Length: " << content.size() << "\r\n";

    if ( fullPath.find( ".html" ) != std::string::npos )
        response_stream << "Content-Type: text/html\r\n";
    else if ( fullPath.find( ".jpg" ) != std::string::npos )
        response_stream << "Content-Type: image/jpeg\r\n";
    else if ( fullPath.find( ".ico" ) != std::string::npos )
        response_stream << "Content-Type: image/x-icon\r\n";

    response_stream << "\r\n" << content;

    data.response = response_stream.str();

    return ;
}