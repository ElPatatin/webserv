/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_get.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:19:25 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/16 14:47:38 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parser.hpp"

static void parseComment( std::string * line );

ConfigFile  conf_get( std::fstream * config_file )
{
    ConfigFile  config;
    std::string line;

    while ( std::getline( *config_file, line ) )
    {
        ft::trim( line );
        if ( line[0] == '#' || line.empty() )
            continue ;
        parseComment( &line );

        config.push_back( line );
    }

    return ( config );
}

static void parseComment( std::string * line )
{
    size_t pos = line->find( "#" );
    if ( pos != std::string::npos )
        *line = line->substr( 0, pos );
    ft::rtrim( *line );

    return ;
}