/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:22:05 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 23:59:37 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http( void ) { return ; }

Http::Http( const Http & src ) : HttpRequestParser( src ) { return ; }

Http & Http::operator=( const Http & rhs )
{
    if ( this != &rhs )
    {
        // some code
    }
    return *this;
}

Http::~Http( void ) { return ; }


// Takes the request and the config data and handles the request
// If the request is a redirection, it will redirect the request
// Else if the request is a CGI request, it will handle the CGI request
// Else, it will handle the request as the request method
void Http::handleRequest( const std::string & request, const ConfigData & config_data, const Data & data )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Handling request" );

    try
    {
        Request request_data = Http::parseRequest( request );

        if ( Http::handleRedirect( request_data, config_data, data ) )
            return ;

        Http_t http_data = Http::getHttpData( request_data, config_data );

        if ( request_data.isCGI )
            return ( Http::handleCGI( request_data, config_data, data, http_data.full_path ) );

        Http::handleMethod( request_data, config_data, data, http_data );
    }
    catch ( HttpException & e ) { LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, e.what() ); }
    catch ( std::exception & e ) { LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, e.what() ); }

    return ;
}

Http::Request Http::parseRequest( const std::string & request )
{
    Request request_data = HttpRequestParser::deserializedRequest( request );
    return ( request_data );
}

Http::Http_t    Http::getHttpData( const Http::Request & request, const ConfigData & config_data )
{
    Http_t http_data;

    http_data.endpoint = Http::getEndpoint( request.url );
    http_data.location = config_data.getLocation( http_data.endpoint );
    if ( http_data.location.empty() )
        throw HttpException( "Error: location not found" );

    http_data.root = Http::locationFinder( http_data.location, "root" );
    if ( http_data.root.empty() )
        throw HttpException( "Error: root path not found" );

    http_data.full_path = Http::getFullUrl( http_data.root, request.url );
    if ( http_data.full_path.empty() )
        throw HttpException( "Error: full path not found" );

    http_data.dir_list = Http::locationFinder( http_data.location, "directory_listing" ) == "on" ? true : false;

    http_data.autoindex = Http::locationFinder( http_data.location, "autoindex" ) == "on" ? true : false;
    http_data.index_page = Http::locationFinder( http_data.location, "index" );

    return ( http_data );
}

bool    Http::handleRedirect( const Request & request, const ConfigData & config_data, const Data & data )
{
    Redirects redirects = config_data.getRedirects();
    if ( redirects.find( request.url ) != redirects.end() )
    {
        std::pair< unsigned short, std::string > redirect = Http::getRedirect( request.url, redirects );
        HttpFileServing::httpRedirect( const_cast< Data & >( data ), request, redirect.first, redirect.second );
        return ( true );
    }

    return ( false );
}

void    Http::handleMethod( const Request & request_data, const ConfigData & config_data, const Data & data, const Http::Http_t & http_data )
{
    std::string allow_methods = Http::locationFinder( http_data.location, "allow_methods" );
    std::bitset< 9 > allowed_method = HttpMethods::getMethodBitMap( allow_methods );
    if ( ( allowed_method.operator&=( 1 << request_data.method ) ) == 0 )
        return ;

    // Build a funcion table pointer
    void ( *method_table[ 3 ] )( const Request &, const ConfigData &, const Data &, const Http::Http_t & ) = {
        Http::handleGet,
        Http::handlePost,
        Http::handleDelete
    };

    method_table[ request_data.method ]( request_data, config_data, data, http_data );

    return ;
}
