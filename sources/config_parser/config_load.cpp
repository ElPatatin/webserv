/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_load.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:57:44 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/18 01:10:41 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parser.hpp"
#include <vector>

static void config_select_server_load( const ConfigFile & config, ConfigData * config_data, InfoServer & info );
static void config_checking( std::string line, ConfigData & current_config, size_t i, const ConfigFile & config );
static void get_server_info( const ConfigFile & config, size_t line_num, InfoServer & info );
static void get_server_info1( const ConfigFile & config, ConfigFile::const_iterator & it, size_t line_num, InfoServer & info );

void    config_load( const ConfigFile & config, ConfigData * config_data, size_t n_servers )
{
    InfoServer  info;
    info.line_num = 0;

    // print config file with line numbers
    // for ( size_t i = 0; i < config.size(); ++i )
    //     std::cout << i << ": " << config[ i ] << std::endl;

    for ( size_t i = 0; i < n_servers; ++i )
        config_select_server_load( config, &config_data[ i ], info );

    return ;
}

static void config_select_server_load( const ConfigFile & config, ConfigData * config_data, InfoServer & info )
{
    info.is_virtual = false;
    size_t bracket_level = 0;
    std::vector < ConfigData > virtual_servers;
    ConfigData *current_config_data = config_data;

    for ( size_t i = info.line_num; i < config.size(); ++i )
    {
        if ( config[ i ].find( "{" ) != std::string::npos )
            ++bracket_level;
        if ( config[ i ].find( "}" ) != std::string::npos )
            --bracket_level;

        if ( bracket_level == 0 )
        {
            if ( i == config.size() - 1 )
                break ;
            get_server_info( config, i, info );
            if ( info.is_virtual == true )
            {
                virtual_servers.push_back( ConfigData() );
                current_config_data = &virtual_servers.back();
                info.is_virtual = false;
            }
            else
            {
                current_config_data = config_data;
                return ;
            }
        }
        else
            config_checking( config[ i ], *current_config_data, i, config );
    }

    if ( !virtual_servers.empty() )
        config_data->setVirtualServers( virtual_servers );

    return ;
}



static void   get_server_info( const ConfigFile & config, size_t line_num, InfoServer & info )
{
    for ( ConfigFile::const_iterator it = config.begin() + line_num; it != config.end(); ++it )
    {
        std::vector < std::string > tokens = ft::split( *it, " " );
        if ( tokens.size() == 2 && tokens[ 0 ] == "server" && tokens[ 1 ] == "{" )
        {
            get_server_info1( config, ++it, line_num, info );
            if ( info.is_virtual == false )
                break ;
        }
    }

    return ;
}

static void get_server_info1( const ConfigFile & config, ConfigFile::const_iterator & it, size_t line_num, InfoServer & info )
{
    static std::vector < std::string >  ports;
    std::string                         current_port;
    int                                 bracket_level = 1;
    std::vector < std::string >         tokens;

    while ( it != config.end() )
    {
        tokens = ft::split( *it, " " );
        if ( tokens.size() == 2 && tokens[1] == "{" )
            ++bracket_level;

        if ( tokens.size() == 2 && tokens[0] == "listen" )
        {
            current_port = tokens[1];
            if ( std::find( ports.begin(), ports.end(), current_port ) == ports.end() )
            {
                ports.push_back( current_port );
                info.line_num = ++line_num;
                info.is_virtual = false;
                break ;
            }
            else
            {
                info.line_num = ++line_num;
                info.is_virtual = true;
                break ;
            }

            if ( ports.back() != current_port )
                throw ConfigFileException( "Error: port " + current_port + " is already in use." );
        }

        if ( bracket_level == 1 && ( tokens.size() == 1 && tokens[0] == "}" ) )
            break ;
        ++it;
    }

    return ;
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

    if ( line.find( "directory_listing" ) != std::string::npos )
    {
        if ( !ConfigLoad::config_load_directory_listing( line, current_config ) )
            throw ConfigFileException( "Error: could not load directory listing." );
    }

    return ;
}