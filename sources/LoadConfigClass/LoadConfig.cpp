/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:59:40 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/02 13:13:10 by cpeset-c         ###   ########.fr       */
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


/*
 * @brief Sets the config path to the given or the default one.
 * Opens the config file, reads it, and close it, if possible.
*/
void LoadConfig::loadConfig( int ac, char **av, ConfigData *config )
{
    std::string     config_path;
    std::fstream *  config_file;

    if (ac == 1)
        config_path = DEFAULT_CONF_PATH;
    else
        config_path = av[1];

    if ( config_path.length() < 6 )
        throw FileNotConfigException( "Error: " + config_path + " is not a .conf file" );
    if ( ".conf" != config_path.substr( config_path.find_last_of( '.' ) ) )
        throw FileNotConfigException( "Error: " + config_path + " is not a .conf file" );

    config_file = ft::openFile( config_path, std::ios::in | std::ios::out );
    if ( !config_file )
        throw ConfigFileException( "Error: could not open config file" );
    readConfig( config_file, config );
    ft::closeFile( config_file );

    return ;
}

/*
 * @brief Checks if the config file is properly structured.
*/
bool LoadConfig::checkConfig( void )
{
    return ( true );
}

// PRIVATE MEMBER FUNCTIONS
// ========================

/*
 * @brief Reads the config file and parses it into a ConfigData object.
*/
void LoadConfig::readConfig( std::fstream *config_file, ConfigData *config )
{
    std::string line;

    LOG( INFO ) << "Reading config file";
    while ( std::getline( *config_file, line ) )
    {
        ft::trim( line );
        if ( line[0] == '#' || line.empty() )
            continue ;
        ConfigParser::parseComment( &line );

        if ( line.find( "listen" ) != std::string::npos )
        {
            if ( !ConfigParser::parsePort( line, config ) )
                { LOG( ERROR ) << "Error parsing port"; continue ; }
        }
        
        if ( line.find( "server_name" ) != std::string::npos )
        {
            if ( !ConfigParser::parseServerName( line, config ) )
                { LOG( ERROR ) << "Error parsing server name"; continue ; }
        }

        if ( line.find( "error_page" ) != std::string::npos )
        {
            if ( !ConfigParser::parseErrorPage( line, config ) )
                { LOG( ERROR ) << "Error parsing error page"; continue ; }
        }

        if ( line.find( "client_max_body_size" ) != std::string::npos )
        {
            if ( !ConfigParser::parseClientMaxBodySize( line, config ) )
                { LOG( ERROR ) << "Error parsing client max body size"; continue ; }
        }

    }

    LOG(INFO) << config->toString();
    return ;
}