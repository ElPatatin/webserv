/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:23:15 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 16:55:49 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGDATA_HPP
# define CONFIGDATA_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <map>

# define UNUSED(x) (void)(x)

// Forward declaration of ConfigData class for the typedef
class ConfigData;

typedef unsigned short Port;
typedef std::vector< std::string > ServerNames;
typedef std::map< int, std::string > ErrorPages;
typedef std::vector< std::map< std::string, std::string > > Locations;
typedef std::vector< ConfigData > NestedServers;


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
        // ==========================
        ConfigData( );
        ~ConfigData( );
        ConfigData( ConfigData const & src );

        // OPERATORS OVERLOAD
        // ==================
        ConfigData & operator=( ConfigData const & rhs );

        // ATTRIBUTES
        // ==========
        Port            port;
        std::string     host;
        ServerNames     server_names;
        ErrorPages      error_pages;
        std::string     client_max_body_size;
        Locations       locations;              // Each location is a map of settings
        NestedServers   nested_servers;         // Nested servers if any

        // MEMBER FUNCTIONS
        // ================
        void            print( void ) const;
        void            clear( void );
        std::string     toString( void ) const;

        // ACCESSORS
        // =========
        Port            getPort( void ) const;
        void            setPort( Port port );

        std::string     getHost( void ) const;
        void            setHost( std::string host );

        ServerNames     getServerNames( void ) const;
        void            setServerNames( ServerNames server_names );

        ErrorPages      getErrorPages( void ) const;
        void            setErrorPages( ErrorPages error_pages );

        std::string     getClientMaxBodySize( void ) const;
        void            setClientMaxBodySize( std::string client_max_body_size );

        Locations       getLocations( void ) const;
        void            setLocations( Locations );

        NestedServers   getNestedServers( void ) const;
        void            setNestedServers( NestedServers nested_servers );
};

#endif