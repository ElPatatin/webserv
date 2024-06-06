/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:59:40 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/06 12:34:32 by cpeset-c         ###   ########.fr       */
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

std::map<std::string, std::string> LoadConfig::loadConfig( int ac, char **av )
{
    std::string configPath;
    std::map<std::string, std::string> config;
    std::fstream * configFile;

    if (ac == 1)
        configPath = DEFCONFPATH;
    else
        configPath = av[1];
    configFile = openConfig( configPath );
    config = parseConfig( configFile );
    closeConfig( configFile );
    return ( config );
}

// PRIVATE MEMBER FUNCTIONS
// ========================

std::fstream * LoadConfig::openConfig( std::string configPath )
{
    try
    {
        std::fstream * configFile = new std::fstream;

        configFile->open( configPath, std::ios::in );
        if (!configFile->is_open())
            throw FileNotOpenException( "Error: " + configPath + " not found" );
        
        return ( configFile );
    }
    catch (std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return ( nullptr );
    }
}

std::map<std::string, std::string> LoadConfig::parseConfig( std::fstream * configFile )
{
    std::map<std::string, std::string> config;
    std::string line;
    std::string key;
    std::string value;

    while (std::getline( *configFile, line ))
    {
        if (line[0] == '#' || line.empty())
            continue ;
        key = line.substr( 0, line.find( '=' ));
        value = line.substr( line.find( '=' ) + 1 );
        config[key] = value;
    }

    return ( config );
}

void LoadConfig::closeConfig( std::fstream * configFile )
{
    try
    {
        configFile->close();
        delete configFile;
        return ;
    }
    catch (std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
}

// EXCEPTIONS
// ==========

LoadConfig::FileNotOpenException::FileNotOpenException( std::string const & message )\
    : std::runtime_error( message ) { return ; }