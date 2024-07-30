/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 18:36:44 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 19:14:41 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

void Http::handlePost( const Request & request, const ConfigData & config_data, const Data & data, const Http::Http_t & http_data )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "POST request" );
    struct stat info;

    if ( stat( http_data.full_path.c_str(), &info ) != 0 )
        return ( HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, NOT_FOUND, config_data ) );

    return ( HttpFileServing::httpSaveFile( const_cast< Data & >( data ), request, http_data.full_path, http_data.root ) );
}
