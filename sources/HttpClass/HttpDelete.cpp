/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 18:36:37 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 19:14:45 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

void Http::handleDelete( const Request & request, const ConfigData & config_data, const Data & data, const Http::Http_t & http_data )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "DELETE request" );
    struct stat info;

    if ( stat( http_data.full_path.c_str(), &info ) != 0 && access( http_data.full_path.c_str(), F_OK ) == -1 )
        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, NOT_FOUND, config_data );
    
    if ( info.st_mode & S_IFREG )
    {
        if ( std::remove( http_data.full_path.c_str() ) != 0 )
            return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data ) );
        else
            return ( HttpFileServing::httpDataServing( const_cast< Data & >( data ), request, OK, "" ) );
    }
    else if ( info.st_mode & S_IFDIR )
    {
        if ( access( http_data.full_path.c_str(), W_OK ) == -1 )
            return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, FORBIDDEN, config_data ) );

        if ( http_data.full_path[ http_data.full_path.size() - 1 ] == '/' )
        {
            std::string command = "rm -fr " + http_data.full_path + "*";
            if ( std::system( command.c_str() ) != 0 )
                return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data ) );
            else
                return ( HttpFileServing::httpDataServing( const_cast< Data & >( data ), request, OK, "" ) );
        }
        
        if ( std::remove( http_data.full_path.c_str() ) != 0 )
            return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data ) );
        else
            return ( HttpFileServing::httpDataServing( const_cast< Data & >( data ), request, OK, "" ) );
    }

    return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data ) );
}
