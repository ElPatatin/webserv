/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:59:40 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/20 19:52:47 by cpeset-c         ###   ########.fr       */
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
        config_path = DEFAULT_CONF_PATH;
    else
        config_path = av[1];
    config_file = openConfig( config_path );
    config = parseConfig( config_file );
    closeConfig( config_file );
    return ( config );
}

bool LoadConfig::checkConfig( std::map<std::string, FieldInterface *> config )
{
    static const std::string required_keys[] = { "host", "port" };
    static const std::string optional_keys[] = { "server_names", "location" };
    const size_t required_keys_count = sizeof( required_keys ) / sizeof( required_keys[0] );

    // Check for presence and validity of required keys
    for ( size_t i = 0; i < required_keys_count; ++i )
    {
        // Check if key is present
        const std::string &key = required_keys[i];
        std::map<std::string, FieldInterface *>::const_iterator it = config.find( key );
        if ( it == config.end() || it->second == NULL )
            return ( false );

        // Check if key is not empty
        Field<std::string> *field = dynamic_cast<Field<std::string> *>(it->second);
        if ( field && field->getValue().empty() )
            return ( false );
    }

    // Check if ALL required keys are present
    for ( size_t i = 0; i < required_keys_count; ++i )
    {
        if ( config.find( required_keys[i] ) == config.end() )
            return ( false );
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

    while ( std::getline( *config_file, line ) )
    {
        if ( line[0] == '#' || line.empty() )
            continue;

        std::string key = line.substr( 0, line.find( '=' ) );
        std::string value = line.substr( line.find( '=' ) + 1 );

        bool is_number = true;
        for ( std::string::const_iterator it = value.begin(); it != value.end(); ++it )
        {
            if ( !std::isdigit( *it ) )
            {
                is_number = false;
                break;
            }
        }

        if ( is_number )
        {
            int int_value;
            std::istringstream( value ) >> int_value;
            if ( key == "port" )
                config[key] = new Field<u_int16_t>( int_value );
            else
                config[key] = new Field<int>( int_value );
        }
        else if ( value[0] == '[' && value[value.length() - 1] == ']' )
        {
            std::string array = value.substr( 1, value.length() - 2 );
            t_vecstr array_values;
            size_t start = 0;
            size_t end = 0;
            while ( ( end = array.find( ',', start ) ) != std::string::npos )
            {
                array_values.push_back( array.substr( start, end - start ) );
                start = end + 1;
            }
            array_values.push_back( array.substr( start ) );
            config[key] = new Field<t_vecstr >( array_values );
        }
        else
        {
            std::string stripped_value = value;
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

// Specialization of printValue for t_vecstr
template <>
void Field<t_vecstr >::printValue() const
{
    std::cout << "[";
    for (size_t i = 0; i < _value.size(); ++i)
    {
        std::cout << _value[i];
        if (i != _value.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]";
}