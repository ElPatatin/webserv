/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 12:08:17 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/24 15:59:05 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser( ) { return ; }

ConfigParser::ConfigParser( ConfigParser const & src ) { UNUSED(src); return ; }

ConfigParser::~ConfigParser( ) { return ; }

ConfigParser & ConfigParser::operator=( ConfigParser const & rhs ) { UNUSED(rhs); return *this; }

std::string ConfigParser::parseLine( std::string * line, std::string error_message )
{
    size_t pos = line->find( " " );
    if ( pos == std::string::npos )
    {
        std::cerr << "Error: " << error_message << std::endl;
        LOG( ERROR ) << error_message;
        return ( "" );
    }

    *line = line->substr( pos + 1 );
    while ( !line->empty() && (*line)[line->size() - 1] == ';')
        line->erase(line->size() - 1);
    
    return ( *line );
}

void ConfigParser::parseComment( std::string * line )
{
    size_t pos = line->find( "#" );
    if ( pos != std::string::npos )
        *line = line->substr( 0, pos );
    ft::rtrim( *line );

    return ;
}

bool ConfigParser::parsePort( std::string line, ConfigData *config )
{
    parseLine( &line, "'port' directive without port number" );
    if ( line.empty() )
        return ( false );

    unsigned short port = std::atoi( line.c_str() );

    std::stringstream ss;
    ss << port;
    if ( ss.str() != line  || ( port <= 0 || port > USHRT_MAX ) )
    {
        std::cerr << "Error: Invalid port number: " << port << std::endl;
        LOG( ERROR ) << "Invalid port number: " << port;
        return ( false );
    }

    config->setPort( static_cast< unsigned short >( port ));
    LOG( INFO ) << "Successfully parsed port: " << port;
    return ( true );
}

bool ConfigParser::parseServerName( std::string line, ConfigData *config )
{

    parseLine( &line, "'server_name' directive without server name" );
    if ( line.empty() )
        return ( false );

    std::vector< std::string > server_names = ft::split( line, ' ' );

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
    parseLine( &line, "'error_page' directive without error code and/or path" );
    if ( line.empty() )
        return ( false );

    ErrorPages error_pages;

    std::vector< std::string > error_page = ft::split( line, ' ' );
    if ( error_page.size() != 2 )
    {
        std::cerr << "Error: 'error_page' directive with invalid number of arguments" << std::endl;
        LOG( ERROR ) << "'error_page' directive with invalid number of arguments";
        return ( false );
    }

    int error_code = std::atoi( error_page[0].c_str() );
    if ( error_code <= 0 )
    {
        std::cerr << "Error: Invalid error code: " << error_page[0] << std::endl;
        LOG( ERROR ) << "Invalid error code: " << error_page[0];
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
    parseLine( &line, "'client_max_body_size' directive without size" );
    if ( line.empty() )
        return ( false );

    config->setClientMaxBodySize( line );
    LOG( INFO ) << "Successfully parsed client max body size: " << config->getClientMaxBodySize();
    return ( true );
}