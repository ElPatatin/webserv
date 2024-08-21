/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 18:36:41 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/22 00:24:33 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

void Http::handleGet( const Request & request, const ConfigData & config_data, const Data & data, const Http::Http_t & http_data )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "GET request" );
    struct stat info;

    if ( stat( http_data.full_path.c_str(), &info ) != 0 && access( http_data.full_path.c_str(), F_OK ) == -1 )
        return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, NOT_FOUND, config_data ) );
    if ( access( http_data.full_path.c_str(), R_OK ) == -1 )
        return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, FORBIDDEN, config_data ) );

    if ( info.st_mode & S_IFDIR )
    {
        if ( http_data.autoindex && request.url[ request.url.size() - 1 ] == '/' )
        {
            return ( HttpFileServing::httpFileServing( const_cast< Data & >( data ), config_data, request, OK, http_data.full_path + const_cast< Request & >( request ).url ) );
        }
            
        if ( info.st_mode & S_IFDIR && http_data.dir_list )
            return ( HttpFileServing::httpDirectoryListing( const_cast< Data & >( data ), config_data, request, http_data.full_path ) );
        else if ( info.st_mode & S_IFDIR && !http_data.dir_list )
            return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, UNAUTHORIZED, config_data ) );
    }
  
    else if ( info.st_mode & S_IFREG )
    {
        int response_code = 0;
        ErrorPages error_pages = config_data.getErrorPages();
        if ( error_pages.empty() )
        {
            VirtualServers virtual_server = config_data.getVirtualServers();
            for ( VirtualServers::iterator it = virtual_server.begin(); it != virtual_server.end(); it++ )
            {
                if ( it->getErrorPages().empty() )
                    continue ;
                error_pages = it->getErrorPages();
                break ;
            }
            if ( error_pages.empty() )
                return ( HttpFileServing::httpFileServing( const_cast< Data & >( data ), config_data, request, OK, http_data.full_path ) );
        }
        if ( Http::checkErrorFile( http_data.full_path, error_pages, &response_code ) )
            return ( HttpFileServing::httpFileServing( const_cast< Data & >( data ), config_data, request, response_code, http_data.full_path ) );
        return ( HttpFileServing::httpFileServing( const_cast< Data & >( data ), config_data, request, OK, http_data.full_path ) );
    }

    return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data ) );
}
