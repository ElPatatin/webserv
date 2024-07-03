/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:37:10 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/03 15:19:42 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOADCONFIG_HPP
# define LOADCONFIG_HPP

# include "ConfigData.hpp"
# include "Log.hpp"
# include "Exceptions.hpp"
# include "ConfigParser.hpp"
# include "utils.hpp"

# define DEFAULT_CONF_PATH "./configuration/default.conf"

/**
 * @brief LoadConfig class
 * 
 * This class is used to load a configuration file and parse it into a map of FieldInterface pointers.
 * The configuration file is expected to be in the format of key-value pairs, with one pair per line.   
 * The key is a string and the value can be of any type.
 * 
 * The class provides a static method loadConfig that takes the path to the configuration file as an argument
 * and returns a map of FieldInterface pointers.
 * 
 * The class also provides a static method checkConfig that takes the map of FieldInterface pointers as an argument
 * and checks if all required keys are present and have values.
*/
class LoadConfig
{
    public:
        // MEMBER FUNCTIONS
        // ================

        static void loadConfig( int ac, char **av, ConfigData *config );
        static bool checkConfig( void );

    private:
        // CONSTRUCTORS AND DESTRUCTOR
        // ===========================

        LoadConfig( );
        LoadConfig( LoadConfig const & src );
        ~LoadConfig( );

        // OPERATORS OVERLOAD
        // ==================

        LoadConfig & operator=( LoadConfig const & rhs );

        // MEMBER FUNCTIONS
        // ================

        // ==========================
        static void readConfig( std::fstream * config_file, ConfigData * config );
};

#endif