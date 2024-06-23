/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 12:08:17 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 16:16:34 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser( ) { return ; }

ConfigParser::ConfigParser( ConfigParser const & src ) { UNUSED(src); return ; }

ConfigParser::~ConfigParser( ) { return ; }

ConfigParser & ConfigParser::operator=( ConfigParser const & rhs ) { UNUSED(rhs); return *this; }

void ConfigParser::parseComment( std::string * line )
{
    size_t pos = line->find( "#" );
    if ( pos != std::string::npos )
        *line = line->substr( 0, pos );
    rtrim( *line );

    return ;
}

bool ConfigParser::parsePort( std::string line, ConfigData *config )
{
    size_t pos = line.find( " " );
    if ( pos == std::string::npos )
    {
        std::cerr << "Error: 'listen' directive without port number" << std::endl;
        return ( false );
    }

    line = line.substr( pos + 1 );
    while ( !line.empty() && line[line.size() - 1] == ';')
        line.erase(line.size() - 1);

    unsigned short port = std::atoi( line.c_str() );

    std::stringstream ss;
    ss << port;
    if ( ss.str() != line  || ( port <= 0 || port > USHRT_MAX ) )
    {
        std::cerr << "Error: Invalid port number: " << port << std::endl;
        return ( false );
    }

    config->setPort( static_cast< unsigned short >( port ));
    LOG( INFO ) << "Successfully parsed port: " << port;
    return ( true );
}

bool ConfigParser::parseServerName( std::string line, ConfigData *config )
{
    size_t pos = line.find( " " );
    if ( pos == std::string::npos )
    {
        std::cerr << "Error: 'server_name' directive without server name" << std::endl;
        return ( false );
    }

    line = line.substr( pos + 1 );
    while ( !line.empty() && line[line.size() - 1] == ';')
        line.erase(line.size() - 1);

    std::vector< std::string > server_names = split( line, ' ' );

    config->setHost( server_names[0] );
    config->setServerNames( server_names );
    LOG( INFO ) << "Successfully parsed host: " << server_names[0];
    LOG( INFO ) << "Successfully parsed server names: ";
    for ( size_t i = 0; i < server_names.size(); ++i )
        LOG(INFO) << "    " << server_names[i];
    return ( true );
}

bool ConfigParser::parseErrorPage( std::string line, ConfigData *config )
{
    UNUSED(config);
    size_t pos = line.find( " " );
    if ( pos == std::string::npos )
    {
        std::cerr << "Error: 'error_page' directive without error code" << std::endl;
        return ( false );
    }

    line = line.substr( pos + 1 );
    while ( !line.empty() && line[line.size() - 1] == ';')
        line.erase(line.size() - 1);

    ErrorPages error_pages;

    std::vector< std::string > error_page = split( line, ' ' );
    if ( error_page.size() != 2 )
    {
        std::cerr << "Error: 'error_page' directive with invalid number of arguments" << std::endl;
        return ( false );
    }

    int error_code = std::atoi( error_page[0].c_str() );
    if ( error_code <= 0 )
    {
        std::cerr << "Error: Invalid error code: " << error_page[0] << std::endl;
        return ( false );
    }

    error_pages[error_code] = error_page[1];
    config->setErrorPages( error_pages );
    LOG( INFO ) << "Successfully parsed error page: ";
    for ( std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it )
        LOG(INFO) << "    " << it->first << " -> " << it->second;
    return ( true );
}

bool ConfigParser::parseClientMaxBodySize( std::string line, ConfigData *config )
{
    size_t pos = line.find( " " );
    if ( pos == std::string::npos )
    {
        std::cerr << "Error: 'client_max_body_size' directive without size" << std::endl;
        return ( false );
    }

    line = line.substr( pos + 1 );
    while ( !line.empty() && line[line.size() - 1] == ';')
        line.erase(line.size() - 1);

    config->setClientMaxBodySize( line );
    LOG( INFO ) << "Successfully parsed client max body size: " << config->getClientMaxBodySize();
    return ( true );
}