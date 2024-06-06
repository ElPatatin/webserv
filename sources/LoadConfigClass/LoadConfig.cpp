/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:59:40 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/06 16:23:56 by cpeset-c         ###   ########.fr       */
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
    std::string configPath;
    std::map<std::string, FieldInterface *> config;
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

bool LoadConfig::checkConfig( std::map<std::string, FieldInterface *> config )
{
    static const std::string requiredKeys[] = {"port", "host"};

    // Check if all required keys are present and have values
    for (size_t i = 0; i < sizeof(requiredKeys) / sizeof(requiredKeys[0]); ++i)
    {
        const std::string &key = requiredKeys[i];
        if (config.find(key) == config.end() || config[key] == NULL) {
            // Key not found or associated value is NULL
            return ( false );
        }

        // For string values, check if it's empty
        Field<std::string> *strField = dynamic_cast<Field<std::string> *>(config[key]);
        if (strField && strField->getValue().empty())
            return ( false );
    }

    // Check if there are any unexpected keys in the config map
    for ( std::map<std::string, FieldInterface *>::const_iterator it = config.begin(); it != config.end(); ++it ) {
        bool isExpectedKey = false;
        for ( size_t i = 0; i < sizeof(requiredKeys) / sizeof(requiredKeys[0]); ++i )
        {
            if ( it->first == requiredKeys[i] )
            {
                isExpectedKey = true;
                break ;
            }
        }
        if ( !isExpectedKey )
            return ( false );
    }

    return ( true );
}

// PRIVATE MEMBER FUNCTIONS
// ========================

std::fstream * LoadConfig::openConfig( std::string configPath )
{
    std::fstream * configFile = new std::fstream;

    configFile->open( static_cast<const char *>(configPath.c_str()), std::ios::in );
    if (configFile->fail())
        throw FileNotOpenException( "Error: " + configPath + " could not be opened" );
    if (!configFile->is_open())
        throw FileNotOpenException( "Error: " + configPath + " not found" );
        
    return ( configFile );
}

std::map<std::string, FieldInterface *> LoadConfig::parseConfig(std::fstream *configFile) {
    std::map<std::string, FieldInterface *> config;
    std::string line;

    while (std::getline(*configFile, line))
    {

        if (line[0] == ';' || line.empty() || (line[0] == '[' && line[line.size() - 1] == ']'))
            continue;

        std::string key = line.substr(0, line.find('='));
        std::string value = line.substr(line.find('=') + 1);

        bool isNumber = true;
        for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
        {
            if (!std::isdigit(*it))
            {
                isNumber = false;
                break;
            }
        }

        if (isNumber)
        {
            int intValue;
            std::istringstream(value) >> intValue;
            config[key] = new Field<int>(intValue);
        }
        else
        {
            if (value.size() >= 2 && value[0] == '"' && value[value.size() - 1] == '"')
            {
                std::string strippedValue = value.substr(1, value.size() - 2);
                config[key] = new Field<std::string>(strippedValue);
            }
            else
            {
                // config[key] = new Field<std::string>(value);
                ; // TODO: Handle other types
            }
        }
    }

    return config;
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
        throw std::runtime_error( e.what() );
    }
}

// EXCEPTIONS
// ==========

LoadConfig::FileNotOpenException::FileNotOpenException( std::string const & message )
    : std::runtime_error( message ) { return ; }