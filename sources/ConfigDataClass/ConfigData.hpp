/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:23:15 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/22 19:54:08 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGDATA_HPP
# define CONFIGDATA_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

# define UNUSED(x) (void)(x)

typedef std::map< std::string, std::string > Location;

/**
 * @brief ConfigData class
 * 
 * This class is used to store the configuration data of a server.
 * 
 * The class provides a print method that prints the configuration data to the standard output.
 * 
 * The class provides a clear method that clears the configuration data.
*/
class ConfigData
{
    public:
        // CONSTRUCTORS AND DESTRUCTOR
        ConfigData( );
        ~ConfigData( );
        ConfigData( ConfigData const & src );

        // OPERATORS OVERLOAD
        // ==================
        ConfigData & operator=( ConfigData const & rhs );

        // ATTRIBUTES
        // ==========
        unsigned short                  port;
        std::string                     host;
        std::vector< std::string >      server_names;
        std::map< int, std::string >    error_pages;
        std::string                     client_max_body_size;
        std::vector< Location >         locations;              // Each location is a map of settings
        std::vector< ConfigData >       nested_servers;         // Nested servers if any

        // MEMBER FUNCTIONS
        // ================
        void                            print( void ) const;
        void                            clear( void );

        // ACCESSORS
        // =========
        unsigned short                  getPort( void ) const;
        void                            setPort( unsigned short port );

        std::string                     getHost( void ) const;
        void                            setHost( std::string host );

        std::vector< std::string >      getServerNames( void ) const;
        void                            setServerNames( std::vector< std::string > server_names );

        std::map< int, std::string >    getErrorPages( void ) const;
        void                            setErrorPages( std::map< int, std::string > error_pages );

        std::string                     getClientMaxBodySize( void ) const;
        void                            setClientMaxBodySize( std::string client_max_body_size );

        std::vector< Location >         getLocations( void ) const;
        void                            setLocations( std::vector< Location > locations );

        std::vector< ConfigData >       getNestedServers( void ) const;
        void                            setNestedServers( std::vector< ConfigData > nested_servers );
};

#endif