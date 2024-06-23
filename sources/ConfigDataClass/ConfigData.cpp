/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 22:52:21 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 16:09:15 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigData.hpp"

ConfigData::ConfigData( )
    : port(0)
{
    this->host.clear();
    this->server_names.clear();
    this->error_pages.clear();
    this->client_max_body_size.clear();
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
    this->client_max_body_size.clear();
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
    oss << "Locations: " << std::endl;
    for ( size_t i = 0; i < this->locations.size(); ++i )
    {
        for ( std::map<std::string, std::string>::const_iterator it = this->locations[i].begin(); it != this->locations[i].end(); ++it )
            oss << "    " << it->first << " -> " << it->second << std::endl;
    }

    if ( !this->nested_servers.empty() )
    {
        oss << "Nested Servers: " << std::endl;
        for (size_t i = 0; i < this->nested_servers.size(); ++i)
        {
            oss << "  Nested Server " << i + 1 << ": " << std::endl;
            oss << this->nested_servers[i].toString();
        }
    }

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

std::string ConfigData::getClientMaxBodySize( void ) const { return ( this->client_max_body_size ); }
void ConfigData::setClientMaxBodySize( std::string client_max_body_size ) { this->client_max_body_size = client_max_body_size; return ; }