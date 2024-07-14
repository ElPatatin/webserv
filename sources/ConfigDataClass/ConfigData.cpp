/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 22:52:21 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/14 19:30:57 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigData.hpp"

ConfigData::ConfigData( )
    : port(0)
{
    this->host.clear();
    this->server_names.clear();
    this->error_pages.clear();
    this->client_max_body_size = 0;
    this->locations.clear();
    this->nested_servers.clear();

    return ;
}

ConfigData::ConfigData( ConfigData const & src )
{
    *this = src;
    return ;
}

ConfigData::~ConfigData( )
{
    this->clear();
    return ;
}

ConfigData & ConfigData::operator=( ConfigData const & rhs )
{
    if ( this != &rhs )
    {
        this->port = rhs.port;
        this->host = rhs.host;
        this->server_names = rhs.server_names;
        this->error_pages = rhs.error_pages;
        this->client_max_body_size = rhs.client_max_body_size;
        this->is_directory_listing = rhs.is_directory_listing;
        this->locations = rhs.locations;
        this->nested_servers = rhs.nested_servers;
    }
    return *this;
}

void ConfigData::print( void ) const
{
    std::cout << this->toString() << std::endl;
    return ;
}

void ConfigData::clear( void )
{
    this->port = 0;
    this->host.clear();
    this->server_names.clear();
    this->error_pages.clear();
    this->client_max_body_size = 0;
    this->locations.clear();
    this->nested_servers.clear();
}

std::string ConfigData::toString( void ) const
{
    std::ostringstream oss;

    oss << "Configuration Data:" << std::endl;
    oss << "Port: " << this->port << std::endl;
    oss << "Host: " << this->host << std::endl;

    oss << "Server Names: ";
    for (size_t i = 0; i < this->server_names.size(); ++i)
        oss << this->server_names[i] << " ";
    oss << std::endl;

    oss << "Error Pages: ";
    for (std::map<int, std::string>::const_iterator it = this->error_pages.begin(); it != this->error_pages.end(); ++it)
        oss << it->first << " -> " << it->second << " ";
    oss << std::endl;

    oss << "Client Max Body Size: " << this->client_max_body_size << std::endl;
    oss << "Directory Listing: " << ( this->is_directory_listing ? "enabled" : "disabled" ) << std::endl;

    oss << "Locations: ";
    for (Locations::const_iterator it = locations.begin(); it != locations.end(); ++it)
    {
        oss << std::endl << "    " << it->first << ": ";
        const Location& loc = it->second;
        for (Location::const_iterator locIt = loc.begin(); locIt != loc.end(); ++locIt)
        {
            oss << std::endl << "        " << locIt->first << ": ";
            for (size_t j = 0; j < locIt->second.size(); ++j)
                oss << locIt->second[j] << " ";
        }
    }
    oss << std::endl;

    oss << "Redirects: ";
    for ( Redirects::const_iterator it = this->redirects.begin(); it != this->redirects.end(); ++it )
        oss << std::endl << "    " << it->first << " -> " << it->second.first << " " << it->second.second;

    return ( oss.str() );

}

// ACCESSORS
// =========

Port ConfigData::getPort( void ) const { return ( this->port ); }
void ConfigData::setPort( Port port ) { this->port = port; return ; }

std::string ConfigData::getHost( void ) const { return ( this->host ); }
void ConfigData::setHost( std::string host )
{
    if ( this->getServerNames().empty() )
        this->host = host;
    return ;
}

ServerNames ConfigData::getServerNames( void ) const { return ( this->server_names ); }
void ConfigData::setServerNames( ServerNames server_names )
{
    this->server_names.insert( this->server_names.end(), server_names.begin(), server_names.end() );
    return ;
}

ErrorPages ConfigData::getErrorPages( void ) const { return ( this->error_pages ); }
void ConfigData::setErrorPages( ErrorPages error_pages )
{
    this->error_pages.insert( error_pages.begin(), error_pages.end() );
    return ;
}

size_t  ConfigData::getClientMaxBodySize( void ) const { return ( this->client_max_body_size ); }
void    ConfigData::setClientMaxBodySize( size_t client_max_body_size ) { this->client_max_body_size = client_max_body_size; return ; }

Locations ConfigData::getLocations( void ) const { return ( this->locations ); }
void ConfigData::setLocations( Locations locations )
{
    this->locations.insert( std::make_pair( locations.begin()->first, locations.begin()->second ) );
    return ;
}

Redirects ConfigData::getRedirects( void ) const { return ( this->redirects ); }
void ConfigData::setRedirects( Redirects redirects )
{
    this->redirects.insert( redirects.begin(), redirects.end() );
    return ;
}

NestedServers ConfigData::getNestedServers( void ) const { return ( this->nested_servers ); }
void ConfigData::setNestedServers( NestedServers nested_servers )
{
    this->nested_servers.insert( this->nested_servers.end(), nested_servers.begin(), nested_servers.end() );
    return ;
}

bool ConfigData::getIsDirectoryListing( void ) const { return ( this->is_directory_listing ); }
void ConfigData::setIsDirectoryListing( bool is_directory_listing ) { this->is_directory_listing = is_directory_listing; return ; }