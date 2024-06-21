/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:59:40 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/21 17:06:47 by cpeset-c         ###   ########.fr       */
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

std::vector< std::string > LoadConfig::loadConfig( int ac, char **av )
{
    std::string config_path;
    std::vector< std::string > config;
    std::fstream * config_file;

    if (ac == 1)
        config_path = DEFAULT_CONF_PATH;
    else
        config_path = av[1];

    config_file = openFile( config_path );
    if ( !config_file )
        return ( config );
    config = parseConfig( config_file );
    closeFile( config_file );

    return ( config );
}

bool LoadConfig::checkConfig( std::vector< std::string > config )
{
    UNUSED(config);
    return ( true );
}

// PRIVATE MEMBER FUNCTIONS
// ========================

std::vector< std::string > LoadConfig::parseConfig(std::fstream *config_file)
{
    std::vector< std::string > config;
    std::string line;

    while ( std::getline( *config_file, line ) )
    {
        trim( line );   
        if ( line[0] == '#' || line.empty() )
            continue;

        

       
        std::cout << line << std::endl;
    }

    return ( config );
}

void LoadConfig::trim(std::string& str)
{
    // Find the first non-space character from the beginning
    size_t startpos = str.find_first_not_of( " \t\r\n" );

    // If all characters are spaces, str is empty or spaces only
    if ( std::string::npos == startpos )
    {
        str = "";
        return;
    }

    // Find the first non-space character from the end
    size_t endpos = str.find_last_not_of( " \t\r\n" );

    // Erase everything from the end not found
    str.erase( endpos + 1 );
    // Erase everything from the beginning not found
    str.erase( 0, startpos );
}


