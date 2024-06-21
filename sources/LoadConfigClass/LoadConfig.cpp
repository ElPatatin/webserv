/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:59:40 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/21 15:45:14 by cpeset-c         ###   ########.fr       */
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
    config_file = openConfig( config_path );
    config = parseConfig( config_file );
    closeConfig( config_file );
    return ( config );
}

bool LoadConfig::checkConfig( std::vector< std::string > config )
{
    UNUSED(config);
    return ( true );
}

// PRIVATE MEMBER FUNCTIONS
// ========================

std::fstream * LoadConfig::openConfig( std::string config_path )
{
    std::fstream * config_file;
    try
    {
        if ( config_path.length() < 6 )
            throw FileNotConfigException( "Error: " + config_path + " is not a .conf file" );
        if ( ".conf" != config_path.substr( config_path.find_last_of( '.' ) ) )
            throw FileNotConfigException( "Error: " + config_path + " is not a .conf file" );

        config_file = new std::fstream;
        if ( config_file == NULL )
            throw MemoryAllocationException( "Error: memory allocation failed" );

        config_file->open( static_cast<const char *>( config_path.c_str() ), std::ios::in );
        if ( config_file->fail() )
            throw FileNotOpenException( "Error: " + config_path + " could not be opened" );
        if ( !config_file->is_open() )
            throw FileNotOpenException( "Error: " + config_path + " not found" );
            
        return ( config_file );
    }
    catch ( FileNotConfigException & e )
    {
        std::cerr << e.what() << std::endl;
        return ( deleteOpenFile( config_file ) );
    }
    catch ( MemoryAllocationException & e )
    {
        std::cerr << e.what() << std::endl;
        return ( deleteOpenFile( config_file ) );
    }
    catch ( FileNotOpenException & e )
    {
        std::cerr << e.what() << std::endl;
        return ( deleteOpenFile( config_file ) );
    }
    catch ( ... )
    {
        std::cerr << "Error: unknown exception" << std::endl;
        return ( deleteOpenFile( config_file ) );
    }
}

std::fstream * LoadConfig::deleteOpenFile( std::fstream * config_file )
{
    if ( config_file )
    {
        delete config_file;
        config_file = NULL;
    }
    return ( NULL );
}

std::vector< std::string > LoadConfig::parseConfig(std::fstream *config_file)
{
    std::vector< std::string > config;
    std::string line;

    while ( std::getline( *config_file, line ) )
    {
        if ( line[0] == '#' || line.empty() )
            continue;

       
    }

    return ( config );
}

void LoadConfig::closeConfig( std::fstream * config_file )
{
    if ( config_file )
    {
        try
        {
            if ( config_file->is_open() )
            {
                config_file->clear();
                config_file->close();
            }
            delete config_file;
        }
        catch( const std::exception & e )
        {
            std::cerr << e.what() << std::endl;
            delete config_file;
        }
        catch( ... )
        {
            std::cerr << "Error: unknown exception" << std::endl;
            delete config_file;
        }
        
    }
    return ;
}