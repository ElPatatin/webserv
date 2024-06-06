/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:37:10 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/06 12:34:34 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOADCONFIG_HPP
# define LOADCONFIG_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <map>

# define UNUSED(x) (void)(x)
# define DEFCONFPATH "./Configuration/default.ini"


class LoadConfig
{
    public:
        // MEMBER FUNCTIONS
        // ================

        static std::map<std::string, std::string> loadConfig( int ac, char **av );

    private:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================

        LoadConfig( );
        LoadConfig( LoadConfig const & src );
        ~LoadConfig( );

        // OPERATORS OVERLOAD
        // ==================

        LoadConfig & operator=( LoadConfig const & rhs );

        // MEMBER FUNCTIONS
        // ================

        static std::fstream * openConfig( std::string configPath );
        static std::map<std::string, std::string> parseConfig( std::fstream * configFile );
        static void closeConfig( std::fstream * configFile );

        // ATTRIBUTES
        // ==========

        // EXCEPTIONS
        // ==========

        class FileNotOpenException : public std::runtime_error
        {
            public:
                FileNotOpenException( std::string const & message );
        };
};

#endif