/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:22:05 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 13:29:48 by cpeset-c         ###   ########.fr       */
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

        if ( request_data.isCGI )
        {
            if ( Http::handleCGI( request_data, config_data, data, full_url ) )
                return ;
        }

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

                if ( stat( full_url.c_str(), &info ) != 0 )
                {
                    HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, NOT_FOUND, config_data );
                    return ;
                }

                HttpFileServing::httpSaveFile( const_cast< Data & >( data ), request_data, full_url, root );
                return ;

                break;
            case DELETE:
                LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "DELETE request" );
                std::cout << "DELETE request" << std::endl;

                if ( stat( full_url.c_str(), &info ) != 0 && access( full_url.c_str(), F_OK ) == -1 )
                    HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, NOT_FOUND, config_data );
                
                //  if its a file, try to delete it
                if ( info.st_mode & S_IFREG )
                {
                    if ( std::remove( full_url.c_str() ) != 0 )
                        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, INTERNAL_SERVER_ERROR, config_data );
                    else
                        HttpFileServing::httpDataServing( const_cast< Data & >( data ), request_data, OK, "" );
                    return ;
                }
                else if ( info.st_mode & S_IFDIR )
                {
                    // if the folder have no permissions, return 403
                    if ( access( full_url.c_str(), W_OK ) == -1 )
                    {
                        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, FORBIDDEN, config_data );
                        return ;
                    }
                    // if url ends with /, remove the contents of the directory
                    if ( full_url[ full_url.size() - 1 ] == '/' )
                    {
                        std::string command = "rm -rf " + full_url + "*";
                        if ( std::system( command.c_str() ) != 0 )
                            HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, INTERNAL_SERVER_ERROR, config_data );
                        else
                            HttpFileServing::httpDataServing( const_cast< Data & >( data ), request_data, OK, "" );
                        return ;
                    }
                    // if url does not end with /, remove the directory
                    if ( std::remove( full_url.c_str() ) != 0 )
                        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, INTERNAL_SERVER_ERROR, config_data );
                    else
                        HttpFileServing::httpDataServing( const_cast< Data & >( data ), request_data, OK, "" );
                }

                HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request_data, INTERNAL_SERVER_ERROR, config_data );
                return ;

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

bool    Http::handleCGI( const Request & request, const ConfigData & config_data, const Data & data, std::string & full_url )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Handling CGI" );

    // Temporary file for CGI output
    std::string tmpFilePath = "/tmp/cgi_output.txt";
    std::string command = full_url + " > " + tmpFilePath;

    // Append query string if not empty
    if  ( !request.query.empty() )
        command += " " + request.query;
    std::cout << command << std::endl;

    // Execute the command
    int result = std::system( command.c_str() );
    if  ( result == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to execute CGI script using std::system" );
        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data );
        return ( true );
    }

    // Open the temporary file to read the CGI output
    std::ifstream tmpFile( tmpFilePath.c_str() );
    if  ( !tmpFile.is_open() )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to open temporary file for CGI output" );
        HttpFileServing::httpErrorServing( const_cast< Data & >( data ), request, INTERNAL_SERVER_ERROR, config_data );
        return ( true );
    }

    // Read the content of the temporary file line by line and add to the response stream
    std::stringstream response_stream;
    std::string line;
    while ( std::getline( tmpFile, line ) )
        response_stream << line << "\n";
    tmpFile.close(); // Close the temporary file

    // Save the response in the data structure
    const_cast< Data & >( data ).response = response_stream.str();
    return ( true );
}

std::string Http::getFullUrl( const std::string & root, const std::string & url )
{
    if ( root.empty() || access( root.c_str(), F_OK ) == -1 )
        throw std::runtime_error( "Error: root path is not valid" );

    std::vector< std::string > url_parts = ft::split( url, "/" );
    std::vector< std::string > root_parts = ft::split( root, "/" );

    std::cout << "Root parts: " << root_parts.back() << " URL: " << url_parts[ 1 ] << std::endl;
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
