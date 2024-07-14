/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:58:22 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/14 15:01:32 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFileParser.hpp"

// CONSTRUCTORS AND DESTRUCTOR
// ===========================
ConfigFileParser::ConfigFileParser( ) { return ; }

ConfigFileParser::ConfigFileParser( ConfigFileParser const & src ) { UNUSED(src); return ; }

ConfigFileParser::~ConfigFileParser( ) { return ; }

// OPERATORS OVERLOAD
// ==================
ConfigFileParser & ConfigFileParser::operator=( ConfigFileParser const & rhs ) { UNUSED(rhs); return *this; }

// MEMBER FUNCTIONS
// ================

/**
 * @brief Parses the config file.
*/
bool    ConfigFileParser::parseConfigFile( std::fstream * config_file )
{
    std::string line;

    while ( std::getline( *config_file, line ) )
    {
        ;
    }

    return ( true );
}