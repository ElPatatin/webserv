/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_server_count.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:53:20 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/17 14:29:04 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parser.hpp"

static void check_port( const ConfigFile & config, ConfigFile::iterator & it, std::vector < std::string > & tokens, size_t & n_servers );
static bool is_valid_port( std::string last_port, std::string current_port );

size_t config_server_count( ConfigFile config )
{
    size_t      n_servers = 0;


    for ( ConfigFile::iterator it = config.begin(); it != config.end(); ++it )
    {
        std::vector < std::string > tokens = ft::split( *it, " " );
        if ( tokens.size() == 2 && tokens[0] == "server" && tokens[1] == "{" )
        {
            ++it;
            check_port( config, it, tokens, n_servers );
        }
    }

    return ( n_servers );
}

static void check_port( const ConfigFile & config, ConfigFile::iterator & it, std::vector < std::string > & tokens, size_t & n_servers )
{
    static std::vector < std::string >  ports;
    std::string                         current_port;
    int                                 bracket_level = 1;

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
                ++n_servers;
                continue ;
            }

            if ( !is_valid_port( ports.back(), current_port ) )
                throw ConfigFileException( "Error: port " + current_port + " is already in use." );
        }

        if ( bracket_level == 1 && ( tokens.size() == 1 && tokens[0] == "}" ) )
            break ;
        ++it;
    }
}

static bool is_valid_port( std::string last_port, std::string current_port )
{
    if ( last_port != current_port )
        return ( false );
    return ( true );
}