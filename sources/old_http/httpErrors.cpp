/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpErrors.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:11:20 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/20 17:34:16 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

void    HttpErrors::sendError( Data & data, int status_code, ConfigData & config )
{
    UNUSED( config );

    // Construct the filename
    std::ostringstream filename_stream;
    filename_stream << "./html/errors/" << status_code << ".html";
    std::string filename = filename_stream.str();

    // Open the file
    std::ifstream file_not_found(filename.c_str(), std::ios::in);
    std::string content( ( std::istreambuf_iterator<char>( file_not_found ) ), std::istreambuf_iterator<char>() );
    file_not_found.close();

    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 " << status_code << " " << HttpResponse::toString1( status_code ) << "\r\n"
                    << "Content-Length: " << content.length() << "\r\n"
                    << "Content-Type: text/html\r\n"
                    << "\r\n"
                    << content;

    data.response = response_stream.str();

    return ;
}