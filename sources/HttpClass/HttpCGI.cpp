/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:44:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 17:40:15 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

void    Http::handleCGI( const Request & request, const ConfigData & config_data, const Data & data, std::string & full_url )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Handling CGI" );

    std::string tmp_file_path = "/tmp/cgi_output.txt";
    std::string command = full_url + " > " + tmp_file_path;

    if  ( !request.query.empty() )
        command += " " + request.query;
    std::cout << command << std::endl;

    int result = std::system( command.c_str() );
    std::cout << result << std::endl;
    if  ( result != 0 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to execute CGI script using std::system" );
        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data );
        return ;
    }

    std::ifstream tmp_file( tmp_file_path.c_str() );
    if  ( !tmp_file.is_open() )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to open temporary file for CGI output" );
        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data );
        return ;
    }

    std::stringstream response_stream;

    response_stream << HttpVersion::toString( request.version ) << " " << OK << " " << HttpResponse::toString( OK ) << "\r\n";

    std::string line;
    std::string buffer;
    while ( std::getline( tmp_file, line ) )
        buffer += line + "\n";
    tmp_file.close();

    if ( buffer.empty() )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to read from temporary file for CGI output" );
        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data );
        return ;
    }

    if ( buffer.find( "Content-Type:" ) == std::string::npos && buffer.find( "Content-Length:" ) == std::string::npos )
    {
        response_stream << "Content-Type: plain/text\r\n";
        response_stream << "Content-Length: " << buffer.size() << "\r\n";
        response_stream << "\r\n";
        response_stream << buffer;

    }
    else
    {
        response_stream << buffer;
        response_stream << "\r\n";
    }

    std::cout << response_stream.str() << std::endl;
    const_cast< Data & >( data ).response = response_stream.str();

    std::remove( tmp_file_path.c_str() );
    return ;
}