/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:59:40 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 13:47:13 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LoadConfig.hpp"

// CONSTRUCTORS AND DESTRUCTOR
// ==========================

LoadConfig::LoadConfig( ) { return ; }

LoadConfig::LoadConfig( LoadConfig const & src ) { UNUSED(src); return ; }

LoadConfig::~LoadConfig( ) { return ; }

// OPERATORS OVERLOAD
// ==================

LoadConfig & LoadConfig::operator=( LoadConfig const & rhs ) { UNUSED(rhs); return *this; }

// MEMBER FUNCTIONS
// ================

void LoadConfig::loadConfig( int ac, char **av, ConfigData *config )
{
    std::string     config_path;
    std::fstream *  config_file;

    if (ac == 1)
        config_path = DEFAULT_CONF_PATH;
    else
        config_path = av[1];

    config_file = openFile( config_path );
    if ( !config_file )
        throw ConfigFileException( strerror( errno ) );
    readConfig( config_file, config );
    closeFile( config_file );

    return ;
}

bool LoadConfig::checkConfig( ConfigData config )
{
    UNUSED(config);
    return ( true );
}

// PRIVATE MEMBER FUNCTIONS
// ========================

void LoadConfig::readConfig( std::fstream *config_file, ConfigData *config )
{
    std::string line;

    while ( std::getline( *config_file, line ) )
    {
        trim( line );
        if ( line[0] == '#' || line.empty() )
            continue ;
        ConfigParser::parseComment( &line );

        if ( line.find( "listen" ) != std::string::npos )
        {
            if ( !ConfigParser::parsePort( line, config ) )
                continue ;
        }
        
        if ( line.find( "server_name" ) != std::string::npos )
        {
            if ( !ConfigParser::parseServerName( line, config ) )
                continue ;
        }

        if ( line.find( "error_page" ) != std::string::npos )
        {
            if ( !ConfigParser::parseErrorPage( line, config ) )
                continue ;
        }

        if ( line.find( "client_max_body_size" ) != std::string::npos )
        {
            if ( !ConfigParser::parseClientMaxBodySize( line, config ) )
                continue ;
        }

    }

    config->print();
    return ;
}