/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:10:24 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/09 18:14:00 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

void Http::httpRequest( HttpData & http, Data & data, ConfigData & config )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Request received" );

    std::string path = http.path == "/" ? "/index.html" : http.path;
    std::string fullPath = "./html" + path;

    struct stat info;
    std::string response;
    std::ostringstream response_stream;
    std::string response_body;

    switch ( http.method )
    {
        case ( GET ):
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "GET request" );

            if ( stat( fullPath.c_str(), &info ) != 0 )
                return ( HttpErrors::sendError( data, NOT_FOUND, config ) );            // Check if file exists
            else if ( info.st_mode & S_IFDIR )
                return ( Http::httpDirectoryListing( path, fullPath, data, config ) );  // Handle Directory listing
            else
                return ( Http::httpFileServing( path, fullPath, data, config ) );       // Handle file serving

            break ;
        // case ( POST ):
            // PostRequest( http, data, config );
            // break;
        default:
            LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Method not allowed" );
            HttpErrors::sendError( data, METHOD_NOT_ALLOWED, config );
            break ;
    }
}
