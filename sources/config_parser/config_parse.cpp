/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:11:55 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/31 00:27:50 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parser.hpp"

static std::fstream * get_file( int ac, char **av );

void    config_parser( int ac, char **av, Cluster & cluster )
{
    std::fstream *  config_file;
    ConfigFile      config;

    config_file = get_file( ac, av );
    config = conf_get( config_file );
    ft::closeFile( config_file );
    if ( !config_validate( config ) )
        throw ConfigFileException( "Error: config file is not properly structured." );
    cluster.n_servers = config_server_count( config );
    if ( cluster.n_servers == 0 )
        throw ConfigFileException( "Error: no servers found in config file." );
    cluster.config_data = new ConfigData[ cluster.n_servers ];
    config_load( config, cluster.config_data, cluster.n_servers );
    if ( !config_load_validate( *cluster.config_data ) )
        throw ConfigFileException( "Error: config data is bad." );
    LOG( INFO ) << "Config file loaded successfully";
    return ;
}

static std::fstream * get_file( int ac, char **av )
{
    std::string     config_path;
    std::fstream *  config_file;

    if (ac == 1)
        config_path = DEFAULT_CONF_PATH;
    else
        config_path = av[1];

    config_file = ft::openFile( config_path, std::ios::in | std::ios::out );
    if ( !config_file )
        throw ConfigFileException( "Error: could not open config file to load." );
    
    return ( config_file );
}
