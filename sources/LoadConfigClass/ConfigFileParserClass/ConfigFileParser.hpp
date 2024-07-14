/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:58:24 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/14 15:00:08 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILEPARSER_HPP
# define CONFIGFILEPARSER_HPP

# include "Exceptions.hpp"
# include "utils.hpp"
# include "Log.hpp"

class ConfigFileParser
{
    public:
        // MEMBER FUNCTIONS
        // ================
        static bool parseConfigFile( std::fstream * config_file );

    private:
        // CONSTRUCTORS AND DESTRUCTOR
        // ===========================
        ConfigFileParser( );
        ConfigFileParser( ConfigFileParser const & src );
        ~ConfigFileParser( );

        // OPERATORS OVERLOAD
        // ==================
        ConfigFileParser & operator=( ConfigFileParser const & rhs );

        // MEMBER FUNCTIONS
        // ================
        // static std::string parseLine( std::string * line, std::string error_message );
        // static void parseComment( std::string * line );
};

#endif
