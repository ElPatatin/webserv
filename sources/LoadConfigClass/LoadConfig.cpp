/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:59:40 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/13 18:46:52 by cpeset-c         ###   ########.fr       */
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

std::map<std::string, FieldInterface *> LoadConfig::loadConfig( int ac, char **av )
{
    std::string config_path;
    std::map<std::string, FieldInterface *> config;
    std::fstream * config_file;

    if (ac == 1)
        config_path = DEF_CONF_PATH;
    else
        config_path = av[1];
    config_file = openConfig( config_path );
    config = parseConfig( config_file );
    closeConfig( config_file );
    return ( config );
}

bool LoadConfig::checkConfig( std::map<std::string, FieldInterface *> config )
{
    static const std::string required_keys[] = { "port", "host" };

    for ( size_t i = 0; i < sizeof( required_keys ) / sizeof( required_keys[0] ); ++i )
    {
        // Check if the key is present in the config map
        const std::string &key = required_keys[i];
        if ( config.find( key ) == config.end() || config[key] == NULL )
            return ( false );

        // For string values, check if it's empty
        Field<std::string> *field = dynamic_cast<Field<std::string> *>( config[key] );
        if ( field && field->getValue().empty() )
            return ( false );
    }

    // Check if there are any unexpected keys in the config map
    std::map<std::string, FieldInterface *>::const_iterator it = config.begin();
    std::map<std::string, FieldInterface *>::const_iterator ite = config.end();
    while ( it != ite ) {
        bool is_expected_key = false;
        for ( size_t i = 0; i < sizeof(required_keys) / sizeof(required_keys[0]); ++i )
        {
            if ( it->first == required_keys[i] )
            {
                is_expected_key = true;
                break ;
            }
        }
        if ( !is_expected_key )
            return ( false );
        ++it;
    }

    return ( true );
}

// PRIVATE MEMBER FUNCTIONS
// ========================

std::fstream * LoadConfig::openConfig( std::string config_path )
{
    if ( config_path.length() < 6 )
        throw FileParseException( "Error: " + config_path + " is not a .conf file" );
    if ( ".conf" != config_path.substr( config_path.find_last_of( '.' ) ) )
        throw FileParseException( "Error: " + config_path + " is not a .conf file" );

    std::fstream * config_file = new std::fstream;
    if ( config_file == NULL )
        throw std::runtime_error( "Error: could not allocate memory for config file" );

    config_file->open( static_cast<const char *>( config_path.c_str() ), std::ios::in );
    if ( config_file->fail() )
        throw FileNotOpenException( "Error: " + config_path + " could not be opened" );
    if ( !config_file->is_open() )
        throw FileNotOpenException( "Error: " + config_path + " not found" );
        
    return ( config_file );
}

std::map<std::string, FieldInterface *> LoadConfig::parseConfig(std::fstream *config_file)
{
    std::map<std::string, FieldInterface *> config;
    std::string line;

    while ( std::getline(*config_file, line) )
    {
        if ( line[0] == '#' || line.empty() )
            continue;

        std::string key = line.substr(0, line.find('='));
        std::string value = line.substr(line.find('=') + 1);

        bool is_number = true;
        for ( std::string::const_iterator it = value.begin(); it != value.end(); ++it )
        {
            if (!std::isdigit(*it))
            {
                is_number = false;
                break;
            }
        }

        if (is_number)
        {
            int int_value;
            std::istringstream( value ) >> int_value;
            config[key] = new Field<int>( int_value );
        }
        else
        {
            std::string stripped_value = value.substr( 1, value.size() - 2 );
            config[key] = new Field<std::string>( stripped_value );
        }
    }

    return ( config );
}

void LoadConfig::closeConfig( std::fstream * config_file )
{
    try
    {
        config_file->close();
        delete config_file;
        return ;
    }
    catch ( std::exception & e )
    {
        throw FileNotCloseException( "Error: could not close config file" );
    }
}

// EXCEPTIONS
// ==========

LoadConfig::FileNotOpenException::FileNotOpenException( std::string const & message )
    : std::runtime_error( message ) { return ; }

LoadConfig::FileNotCloseException::FileNotCloseException( std::string const & message )
    : std::runtime_error( message ) { return ; }

LoadConfig::FileParseException::FileParseException( std::string const & message )
    : std::runtime_error( message ) { return ; }