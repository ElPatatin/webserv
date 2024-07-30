/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:44:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 15:41:42 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

bool    Http::handleCGI( const Request & request, const ConfigData & config_data, const Data & data, std::string & full_url )
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
        return ( true );
    }

    std::ifstream tmpFile( tmp_file_path.c_str() );
    if  ( !tmpFile.is_open() )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to open temporary file for CGI output" );
        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data );
        return ( true );
    }

    std::stringstream response_stream;

    response_stream << HttpVersion::toString( request.version ) << " " << OK << " " << HttpResponse::toString( OK ) << "\r\n";
    std::string line;
    while ( std::getline( tmpFile, line ) )
        response_stream << line << "\n";
    tmpFile.close();

    const_cast< Data & >( data ).response = response_stream.str();
    return ( true );
}