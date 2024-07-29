/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:22:05 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/29 19:08:29 by cpeset-c         ###   ########.fr       */
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

Http::Request Http::parseRequest( const std::string & request )
{
    Request request_data = HttpRequestParser::deserializedRequest( request );
    return ( request_data );
}

void Http::handleRequest( const std::string & request, const ConfigData & config_data, const Data & data )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Handling request" );

    try
    {
        Request request_data = HttpRequestParser::deserializedRequest( request );

        Redirects redirects;
        std::pair< unsigned short, std::string > redirect;
        // Check if the requested path is a redirection
        redirects = config_data.getRedirects();
        if ( redirects.find( request_data.url ) != redirects.end() )
        {
            redirect = Http::getRedirect( request_data.url, redirects );
            HttpFileServing::httpRedirect( const_cast< Data & >( data ), request_data, redirect.first, redirect.second );
            return ;
        }

        std::string endpoint = Http::getEndpoint( request_data.url );
        std::cout << "Endpoint: " << endpoint << std::endl;

        Location location = config_data.getLocation( endpoint );
        if ( location.empty() )
            return ;
        std::cout << "Location found" << std::endl;

        // std::string allow_methods = Http::locationFinder( location, "allow_methods" );
        // std::bitset< 9 > allowed_method = HttpMethods::getMethodBitMap( allow_methods );
        // if ( ( allowed_method.operator&=( 1 << request_data.method ) ) == 0 )
        //     return ;

        std::string root = Http::locationFinder( location, "root" );
        std::cout << "Root: " << root << std::endl;
        std::string full_url = Http::getFullUrl( root, request_data.url );
        std::cout << "Full url: " << full_url << std::endl;

        struct stat info;
        bool is_directory = Http::locationFinder( location, "directory_listing" ) == "on" ? true : false;
        switch ( request_data.method )
        {
            case GET:
                LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "GET request" );

                std::cout << "GET request" << std::endl;
                if ( stat( full_url.c_str(), &info ) != 0 && access( full_url.c_str(), F_OK ) == -1 )
                    HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, NOT_FOUND, config_data );
            
                
                if ( info.st_mode & S_IFDIR && is_directory )
                {
                    std::cout << "Sending directory listing" << std::endl;
                    HttpFileServing::httpDirectoryListing( const_cast< Data & >( data ), config_data, request_data, full_url );
                }
                else if ( info.st_mode & S_IFDIR && !is_directory )
                {
                    std::cout << "Unauthorized request, directory listing is off for " << endpoint << std::endl;
                    HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, UNAUTHORIZED, config_data );
                }
                else if ( info.st_mode & S_IFREG )
                {
                    int response_code = 0;
                    ErrorPages error_pages = config_data.getErrorPages();
                    if ( Http::checkErrorFile( full_url, error_pages, &response_code ) )
                    {
                        std::cout << "Sending error file" << std::endl;
                        HttpFileServing::httpFileServing( const_cast< Data & >( data ), config_data, request_data, response_code, full_url );
                    }
                    else
                    {
                        std::cout << "Sending file" << std::endl;
                        HttpFileServing::httpFileServing( const_cast< Data & >( data ), config_data, request_data, OK, full_url );
                    }
                }
                else
                {
                    std::cout << "Internal server error" << std::endl;
                    HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, INTERNAL_SERVER_ERROR, config_data );
                }

                break;
            case POST:
                LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "POST request" );
                std::cout << "POST request" << std::endl;
                break;
            case DELETE:
                LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "DELETE request" );
                std::cout << "DELETE request" << std::endl;
                break;
            default:
                LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Method not allowed" );
                std::cout << "Method not allowed" << std::endl;
                break;
        }
    }
    catch ( HttpException & e ) { LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, e.what() ); }
    catch ( std::exception & e ) { LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, e.what() ); }

    return ;
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

bool    Http::handleCGI( const std::string & request, const ConfigData & config_data, const Data & data )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Handling CGI" );
    UNUSED( request );
    UNUSED( config_data );
    UNUSED( data );

    return ( false );
}

std::string Http::getFullUrl( const std::string & root, const std::string & url )
{
    if ( root.empty() || access( root.c_str(), F_OK ) == -1 )
        throw std::runtime_error( "Error: root path is not valid" );

    std::vector< std::string > url_parts = ft::split( url, "/" );
    std::vector< std::string > root_parts = ft::split( root, "/" );

    std::cout << "Root parts: " << root_parts.back() << " URL: " << url_parts[ 1 ] << std::endl;
    if ( root_parts.back() == url_parts[ 1 ] )
    {
        std::cout << "Erasing" << std::endl;
        url_parts.erase( url_parts.begin() + 1 );
    }
    
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
