/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_load.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:57:44 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/21 23:24:54 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parser.hpp"
#include <vector>

static void config_setup_servers( const ConfigFile & config, std::vector < ConfigFile > & servers, size_t & line_num );
static bool config_setup_checking( const ConfigFile & config, std::string port, size_t i );
static void config_select_server_load( const ConfigFile & config, ConfigData * config_data );
static void config_checking( std::string line, ConfigData & current_config, size_t i, const ConfigFile & config );

void    config_load( const ConfigFile & config, ConfigData * config_data, size_t n_servers )
{
    std::vector < ConfigFile > servers;
    UNUSED( config_data );
    size_t  line_num = 0;

    for ( size_t i = 0; i < n_servers; ++i )
    {
        config_setup_servers( config, servers, line_num );
        config_select_server_load( servers[i], &config_data[ i ] );
    }

    return ;
}

static void config_setup_servers( const ConfigFile & config, std::vector < ConfigFile > & servers, size_t & line_num )
{
    ConfigFile  server;
    size_t  brackets = 0;
    std::string port = "";

    for ( size_t j = line_num; j < config.size(); ++j )
    {
        if ( port.empty() && config[ j ].find( "listen" ) != std::string::npos )
            port = config[ j ].substr( config[ j ].find( " " ) + 1 );

        if ( config[ j ].find( "{" ) != std::string::npos )
            ++brackets;
        if ( config[ j ].find( "}" ) != std::string::npos )
            --brackets;

        server.push_back( config[ j ] );
        ++line_num;
        if ( config_setup_checking( config, port, j ) )
            continue ;
        if ( brackets == 0 || j == config.size() - 1 )
            break ;
    }

    servers.push_back( server );
    return ;
}

// Takes the current server configuration and loads it into the ConfigData object.
static void config_select_server_load( const ConfigFile & config, ConfigData * config_data )
{
    ConfigData  current_config;
    ConfigData  primary_config;
    size_t  brackets = 0;
    VirtualServers  virtual_servers;

    // 
    for ( size_t i = 0; i < config.size(); ++i )
    {
        if ( config[ i ].find( "{" ) != std::string::npos )
            ++brackets;
        else if ( config[ i ].find( "}" ) != std::string::npos )
            --brackets;
        
        if  ( brackets )
        {
            config_checking( config[ i ], current_config, i, config );
            continue ;
        }

        if ( primary_config.isEmpty() )
            primary_config = current_config;
        else
        {
            virtual_servers.push_back( ConfigData() );
            virtual_servers.back() = current_config;
        }

        current_config.clear();
        if ( config.size() - 1 == i )
            break ;
    }

    if ( !virtual_servers.empty() )
        primary_config.setVirtualServers( virtual_servers );

    *config_data = primary_config;
    return ;
}

static bool config_setup_checking( const ConfigFile & config, std::string port, size_t i )
{
    while ( i < config.size() )
    {
        if ( config[ i ].find( "listen" ) != std::string::npos )
        {
            if ( config[ i ].substr( config[ i ].find( " " ) + 1 ) == port )
                return ( true );
            else
                return ( false );
        }
        ++i;
    }
    return ( false );
}


static void config_checking( std::string line, ConfigData & current_config, size_t i, const ConfigFile & config )
{
    if ( line.find( "listen" ) != std::string::npos )
    {
        if ( !ConfigLoad::config_load_port( line, current_config ) )
            throw ConfigFileException( "Error: could not load port." );
    }

    if ( line.find( "server_name" ) != std::string::npos )
    {
        if ( !ConfigLoad::config_load_server_name( line, current_config ) )
            throw ConfigFileException( "Error: could not load server name." );
    }

    if ( line.find( "error_page" ) != std::string::npos )
    {
        if ( !ConfigLoad::config_load_error_page( line, current_config ) )
            throw ConfigFileException( "Error: could not load error page." );
    }

    if ( line.find( "client_max_body_size" ) != std::string::npos )
    {
        if ( !ConfigLoad::config_load_client_max_body_size( line, current_config ) )
            throw ConfigFileException( "Error: could not load client max body size." );
    }

    if ( line.find( "location" ) != std::string::npos )
    {
        if ( !ConfigLoad::config_load_locations( config, line, i, current_config ) )
            throw ConfigFileException( "Error: could not load locations." );
    }

    return ;
}