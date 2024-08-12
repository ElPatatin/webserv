/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 19:02:52 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 20:08:18 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

std::string Http::getFullUrl( const std::string & root, const std::string & url )
{
    if ( root.empty() || access( root.c_str(), F_OK ) == -1 )
        throw std::runtime_error( "Error: root path is not valid" );

    std::vector< std::string > url_parts = ft::split( url, "/" );
    std::vector< std::string > root_parts = ft::split( root, "/" );

    if ( root_parts.back() == url_parts[ 1 ] )
        url_parts.erase( url_parts.begin() + 1 );
    
    std::string full_url = root + ft::join( url_parts, "/" );
    return ( full_url );
}

std::string Http::getEndpoint( const std::string & url )
{
    size_t count = 0;
    std::string endpoint = "";

    for ( std::string::const_iterator it = url.begin(); it != url.end(); ++it )
    {
        if ( *it == '/' )
            count++;
    }

    if ( count == 1 )
        endpoint = "/";
    else
        endpoint = url.substr( 0, url.find( '/', 1 ) );

    return ( endpoint );
}

std::string Http::locationFinder( const Location & location, const std::string & key )
{
    for ( size_t i = 0; i < location.size(); ++i )
    {
        if ( location[ i ].first == key )
            return ( location[ i ].second );
    }
    return ( "" );
}

std::pair< unsigned short, std::string > Http::getRedirect( const std::string & endpoint, const Redirects & redirects )
{
    std::pair< unsigned short, std::string > redirect = redirects.at( endpoint );
    return ( redirect );
}

bool    Http::checkErrorFile( const std::string & full_path, ErrorPages & error_pages, int * response_code )
{
    std::string file = ft::split( full_path, "/" ).back();
    std::string error = file.substr( 0, file.find( '.' ) );
    *response_code = ft::stoi( error );
    std::string error_page = error_pages[ *response_code ];
    if ( error_page.empty() )
        return ( false );
    return ( true );
}
