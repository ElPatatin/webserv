/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 12:08:17 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/22 19:57:28 by cpeset-c         ###   ########.fr       */
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

    config->setServerNames( server_names );
    config->setHost( server_names[0] );
    return ( true );
}