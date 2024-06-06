/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:37:10 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/06 17:04:43 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOADCONFIG_HPP
# define LOADCONFIG_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cerrno>
#include <exception>
#include <sstream>
#include <algorithm>
#include <map>

# define UNUSED(x) (void)(x)
# define DEFCONFPATH "./Configuration/default.ini"

// Interface class
class FieldInterface
{
    public:
        virtual ~FieldInterface( ) { return ; }
        virtual void printValue() const = 0;
};

// Template class for different types
template <typename T>
class Field : public FieldInterface
{
    public:
        Field(const T &value) : _value(value) {}
        T getValue() const { return _value; }
        void printValue() const { std::cout << _value; }
    private:
        T _value;
};

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

        static std::map<std::string, FieldInterface *>  loadConfig( int ac, char **av );
        static bool                                     checkConfig( std::map<std::string, FieldInterface *> config );

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

        static std::fstream *                           openConfig( std::string config_path );
        static std::map<std::string, FieldInterface *>  parseConfig( std::fstream * config_file );
        static void                                     closeConfig( std::fstream * config_file );

        // ATTRIBUTES
        // ==========

        // EXCEPTIONS
        // ==========

        class FileNotOpenException : public std::runtime_error
        {
            public:
                FileNotOpenException( std::string const & message );
        };

        class FileParseException : public std::runtime_error
        {
            public:
                FileParseException( std::string const & message );
        };
};

#endif