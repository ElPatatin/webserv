/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 22:52:21 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/18 00:56:41 by cpeset-c         ###   ########.fr       */
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
    this->is_directory_listing = false;
    this->locations.clear();
    this->redirects.clear();
    this->virtual_servers.clear();

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
        this->redirects = rhs.redirects;
        this->locations = rhs.locations;
        this->virtual_servers = rhs.virtual_servers;
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
    this->virtual_servers.clear();
}

std::string ConfigData::toString( void ) const
{
    std::ostringstream oss;

    // Add separator at the beginning
    oss << YELLOW << "----------------------------------------" << RESET << std::endl;

    oss << GREEN << "Configuration Data:" << RESET << std::endl;
    oss << BLUE << "Port: " << RESET << this->port << std::endl;
    oss << BLUE << "Host: " << RESET << this->host << std::endl;

    oss << BLUE << "Server Names: " << RESET;
    for ( size_t i = 0; i < this->server_names.size(); ++i )
        oss << this->server_names[ i ] << " ";
    oss << std::endl;

    oss << BLUE << "Error Pages: " << RESET;
    for ( std::map<int, std::string>::const_iterator it = this->error_pages.begin(); it != this->error_pages.end(); ++it )
        oss << it->first << " -> " << it->second << " ";
    oss << std::endl;

    oss << BLUE << "Client Max Body Size: " << RESET << this->client_max_body_size << std::endl;
    oss << BLUE << "Directory Listing: " << RESET << ( this->is_directory_listing ? "enabled" : "disabled" ) << std::endl;

    oss << BLUE << "Locations: " << RESET << this->locations.size();
    for ( Locations::const_iterator it = locations.begin(); it != locations.end(); ++it )
    {
        oss << std::endl << "    " << CYAN << it->first << RESET << ": ";
        const Location& loc = it->second;
        for ( Location::const_iterator locIt = loc.begin(); locIt != loc.end(); ++locIt )
        {
            oss << std::endl << "        " << CYAN << locIt->first << RESET << ": ";
            for ( size_t j = 0; j < locIt->second.size(); ++j )
                oss << locIt->second[ j ] << " ";
        }
    }
    oss << std::endl;

    oss << BLUE << "Redirects: " << RESET << this->redirects.size();
    for ( Redirects::const_iterator it = this->redirects.begin(); it != this->redirects.end(); ++it )
    {
        oss << std::endl << "    " << CYAN << it->first << RESET << ": ";
        oss << it->second.first << " -> " << it->second.second;
    }
    
    oss << std::endl;

    oss << BLUE << "Virtual Servers: " << RESET << this->virtual_servers.size();
    for ( size_t i = 0; i < this->virtual_servers.size(); ++i )
    {
        oss << std::endl << "    " << CYAN << "Virtual Server " << i + 1 << ":" << RESET << std::endl;
        oss << this->virtual_servers[ i ].toString();
    }

    oss << std::endl;

    return ( oss.str() );
}

bool ConfigData::isEmpty( void ) const
{
    return ( this->port == 0 && this->host.empty() && this->server_names.empty() && this->error_pages.empty() && this->client_max_body_size == 0 && this->locations.empty() && this->redirects.empty() && this->virtual_servers.empty() && this->is_directory_listing == false );
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

VirtualServers ConfigData::getVirtualServers( void ) const
{
    // if ( this->virtual_servers.empty() )
    //     return ( VirtualServers( 1, ConfigData() ) );
    return ( this->virtual_servers );
}
void ConfigData::setVirtualServers( VirtualServers virtual_servers )
{
    this->virtual_servers.insert( this->virtual_servers.end(), virtual_servers.begin(), virtual_servers.end() );
    // this->virtual_servers.push_back( virtual_servers );
    return ;
}

bool ConfigData::getIsDirectoryListing( void ) const { return ( this->is_directory_listing ); }
void ConfigData::setIsDirectoryListing( bool is_directory_listing ) { this->is_directory_listing = is_directory_listing; return ; }