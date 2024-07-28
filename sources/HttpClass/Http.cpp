/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:22:05 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/28 23:31:13 by cpeset-c         ###   ########.fr       */
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

        // Check if the endpoint have the right method
        // 1.- Check the endpoint on the url and Get the endpoint
        std::string endpoint = Http::getEndpoint( request_data.url );
        std::cout << "Endpoint: " << endpoint << std::endl;

        // 2.- get the respetive location
        Location location = config_data.getLocation( endpoint );
        if ( location.empty() )
            return ;
        std::cout << "Location found" << std::endl;

        // 3.- get the methods
        // first find the allow_methods in the location vector and then get the bitset
        std::string allow_methods = Http::locationFinder( location, "allow_methods" );
        std::bitset<9> allowed_method = HttpMethods::getMethodBitMap( allow_methods );
        
        // 4.- Check if the request method is allowed. If not it will stop the request here.
        if ( ( allowed_method.operator&=( 1 << request_data.method ) ) == 0 )
            return ;

        if ( Http::handleCGI( request, config_data, data ) )
            return ;
        
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
                if ( stat( full_url.c_str(), &info ) != 0 )
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
                    std::cout << "Sending file" << std::endl;
                    HttpFileServing::httpFileServing( const_cast< Data & >( data ), config_data, request_data, OK, full_url );
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
            case PUT:
                LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "PUT request" );
                std::cout << "PUT request" << std::endl;
                break;
            case HEAD:
                LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "HEAD request" );
                std::cout << "HEAD request" << std::endl;
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

bool    Http::handleCGI( const std::string & request, const ConfigData & config_data, const Data & data )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Handling CGI" );
    UNUSED( request );
    UNUSED( config_data );
    UNUSED( data );

    return ( false );
}

// std::string Http::getFullUrl( const std::string & root, const std::string & url )
// {
//     if ( root.empty() || access( root.c_str(), F_OK ) == -1 )
//         throw std::runtime_error( "Error: root path is not valid" );

//     std::vector< std::string > url_parts = ft::split( url, "/" );
//     std::vector< std::string > root_parts = ft::split( root, "/" );
//     std::string full_url = "";

//     std::vector< std::string > new_root_parts;
//     // Compare the root and the url parts to get the full url
//     for ( size_t i = 0; i < root_parts.size(); ++i )
//     {
//         for ( size_t j = 0; j < url_parts.size(); ++j )
//         {
//             if ( root_parts[ i ] != url_parts[ j ] )
//             {
//                 new_root_parts.push_back( root_parts[ i ] );
//                 break ;
//             }
//         }
//     }

//     for ( size_t i = 0; i < new_root_parts.size(); ++i )
//     {
//         full_url += new_root_parts[ i ];
//         if ( i != new_root_parts.size() - 1 )
//             full_url += "/";
//     }


//     return ( full_url );
// }

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



// void    smth()
// {

// }
