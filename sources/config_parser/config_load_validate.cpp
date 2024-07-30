/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_load_validate.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:01:35 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 23:42:59 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parser.hpp"

static std::string locationFinder( const Location & location, const std::string & key );

bool    config_load_validate( ConfigData * & config_data )
{
    if ( config_data->getPort() == 0 )
    {
        LOG( ERROR ) << "Error: port is not valid. " << __LINE__;
        return ( false );
    }

    if ( config_data->getHost().empty() )
        LOG( ERROR ) << "Error: host is not valid. " << __LINE__;
    
    if ( config_data->getServerNames().empty() )
        LOG( ERROR ) << "Error: server_name is not valid. " << __LINE__;

    if ( config_data->getErrorPages().empty() )
        LOG( ERROR ) << "Error: error_page is not valid. " << __LINE__;
    
    if ( config_data->getClientMaxBodySize() == 0 )
    {
        LOG( ERROR ) << "Error: client_max_body_size is not valid. " << __LINE__;
        return ( false );
    }

    if ( config_data->getLocations().empty() )
    {
        LOG( ERROR ) << "Error: locations is not valid. " << __LINE__;
        return ( false );
    }

    std::vector< std::string > location_words;
    location_words.push_back( "root" );
    location_words.push_back( "autoindex" );
    Locations locations = config_data->getLocations();
    for ( Locations::iterator it = locations.begin(); it != locations.end(); ++it )
    {
        if ( it->first.empty() )
        {
            LOG( ERROR ) << "Error: location is not valid. " << __LINE__;
            return ( false );
        }
        Location location = it->second;
        if ( location.empty() )
        {
            LOG( ERROR ) << "Error: location is not valid. " << __LINE__;
            return ( false );
        }
        for ( size_t i = 0; i < location.size(); ++i )
        {
            if ( !location[ i ].first.empty() )
            {
                for ( size_t j = 0; j < location_words.size(); ++j )
                {
                    std::string value = locationFinder( location, location_words[ j ] );
                    if ( value.empty() )
                    {
                        LOG( ERROR ) << "Error: " << location_words[ j ] << " is not valid. " << __LINE__;
                        return ( false );
                    }
                }
            }
        }
    }

    if ( config_data->getRedirects().empty() )
        LOG( ERROR ) << "Error: redirect is not valid. " << __LINE__;
    
    if ( config_data->getVirtualServers().empty() )
        LOG( ERROR ) << "Error: virtual_server is not valid. " << __LINE__;
    
    VirtualServers virtual_servers = config_data->getVirtualServers();
    for ( size_t i = 0; i < virtual_servers.size(); ++i)
    {
        // Check if virtual server is valid
        int rv = config_load_validate( reinterpret_cast< ConfigData * & >( virtual_servers[ i ] ) );
        if ( rv == false )
            return ( false );
    }

    return ( true );
}

static std::string locationFinder( const Location & location, const std::string & key )
{
    for ( size_t i = 0; i < location.size(); ++i )
    {
        if ( location[ i ].first == key )
            return ( location[ i ].second );
    }
    return ( "" );
}
