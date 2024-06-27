/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 12:08:15 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/27 15:02:46 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include "ConfigData.hpp"
# include "Exceptions.hpp"
# include "utils.hpp"
# include "Log.hpp"


class ConfigParser
{
    public:
        // MEMBER FUNCTIONS
        // ================
        static void parseComment( std::string * line );
        static bool parsePort( std::string line, ConfigData *config );
        static bool parseServerName( std::string line, ConfigData *config );
        static bool parseErrorPage( std::string line, ConfigData *config );
        static bool parseClientMaxBodySize( std::string line, ConfigData *config );
        static bool parseLocation( std::string line, ConfigData *config );
        static bool parseNestedServer( std::string line, ConfigData *config );

    private:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================
        ConfigParser( );
        ConfigParser( ConfigParser const & src );
        ~ConfigParser( );

        // OPERATORS OVERLOAD
        // ==================
        ConfigParser & operator=( ConfigParser const & rhs );

        // MEMBER FUNCTIONS
        // ================
        static std::string parseLine( std::string * line, std::string error_message );
};

#endif