/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:23:15 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/29 15:26:18 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGDATA_HPP
# define CONFIGDATA_HPP

# include "common.hpp"
# include <vector>
# include <map>

// Forward declaration of ConfigData class for the typedef
class ConfigData;

typedef struct s_cluster
{
    ConfigData *    config_data;
    size_t          n_servers;
}                   Cluster;

typedef unsigned short Port;
typedef std::map< int, std::string > ErrorPages;
typedef std::vector< std::pair< std::string, std::string > > Location;
typedef std::map< std::string, Location > Locations;
typedef std::map< std::string, std::pair< unsigned short, std::string > > Redirects;
typedef std::vector< ConfigData > VirtualServers;
typedef std::vector< std::string > ServerNames;


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
    private:
        // ATTRIBUTES without '_in_front_cause_we_can_yololo'
        // ========== 
        Port            port;
        std::string     host;
        ServerNames     server_names;
        ErrorPages      error_pages;
        size_t          client_max_body_size;
        Locations       locations;              // Each location is a vector of settings
        Redirects       redirects;              // Each redirect is a map of settings
        VirtualServers  virtual_servers;        // Each virtual server is a vector of settings

    public:
        // CONSTRUCTORS AND DESTRUCTOR
        // ==========================
        ConfigData( );
        ~ConfigData( );
        ConfigData( ConfigData const & src );

        // OPERATORS OVERLOAD
        // ==================
        ConfigData & operator=( ConfigData const & rhs );

        // MEMBER FUNCTIONS
        // ================
        void            print( void ) const;
        void            clear( void );
        std::string     toString( void ) const;
        bool            isEmpty( void ) const;

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

        size_t          getClientMaxBodySize( void ) const;
        void            setClientMaxBodySize( size_t client_max_body_size );

        Location        getLocation( std::string endpoint ) const;
        void            setLocation( std::string endpoint, Location location );

        Locations       getLocations( void ) const;
        void            setLocations( Locations );

        Redirects       getRedirects( void ) const;
        void            setRedirects( Redirects redirects );

        VirtualServers  getVirtualServers( void ) const;
        void            setVirtualServers( VirtualServers virtual_servers );
};

#endif