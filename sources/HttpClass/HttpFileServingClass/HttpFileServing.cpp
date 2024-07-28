/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpFileServing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:01:04 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/28 20:17:56 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpFileServing.hpp"

HttpFileServing::HttpFileServing( void ) { return ; }

HttpFileServing::HttpFileServing( const HttpFileServing & src ) { *this = src; }

HttpFileServing & HttpFileServing::operator=( const HttpFileServing & rhs )
{
    if ( this != &rhs )
    {
        *this = rhs;
    }
    return *this;
}

HttpFileServing::~HttpFileServing( void ) { return ; }

void    HttpFileServing::httpFileServing
(
    Data & data,
    const ConfigData & config,
    const HttpRequestParser::Request & request,
    const int & response_code,
    const std::string & full_path
)
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Serving file" );
    std::fstream * file = ft::openFile( full_path, std::ios::in | std::ios::binary );
    if ( !file || !file->is_open() )
        return ( HttpFileServing::httpErrorServing( data, request, NOT_FOUND, config ) );
    
    std::string content = std::string( ( std::istreambuf_iterator< char >( *file ) ), std::istreambuf_iterator< char >() );

    HttpFileServing::httpDataServing( data, request, response_code, content );
    return ;
}

void    HttpFileServing::httpErrorServing
(
    Data & data,
    const HttpRequestParser::Request & request,
    const int & response_code,
    const ConfigData & config
)
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Serving error" );

    ErrorPages error_pages = config.getErrorPages();
    std::string error_page = error_pages[response_code];
    Location location = config.getLocation( "/" );
    std::string error_file = "";
    for ( size_t i = 0; i < location.size(); ++i )
    {
        if ( location[ i ].first == "root" )
        {
            error_file = location[ i ].second + error_page;
            break ;
        }
    }

    std::fstream * file = ft::openFile( error_file, std::ios::in | std::ios::binary );
    if ( !file || !file->is_open() )
    {
        // Default page
        std::stringstream ss;
        ss << "<html><head><title>" << response_code << " " << HttpResponse::toString( response_code ) << "</title></head>";
        ss << "<body><h1>" << response_code << " " << HttpResponse::toString( response_code ) << "</h1></body></html>";
        return ( HttpFileServing::httpDataServing( data, request, response_code, ss.str() ) );
    }
    
    std::string content = std::string( ( std::istreambuf_iterator< char >( *file ) ), std::istreambuf_iterator< char >() );
    ft::closeFile( file );

    return ( HttpFileServing::httpDataServing( data, request, response_code, content ) );
}

void    HttpFileServing::httpDataServing( Data & data, const HttpRequestParser::Request & request, const int & response_code, const std::string & content )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Serving data" );

    std::ostringstream response_stream;

    response_stream << HttpVersion::toString( request.version ) << " " << response_code << " " << HttpResponse::toString( response_code ) << "\r\n";
    response_stream << "Content-Length: " << content.size() << "\r\n";
    response_stream << "Content-Type: " << HttpFileServing::getContentType( request.url ) << "\r\n";
    response_stream << "\r\n";
    response_stream << content;

    // Store response in data
    data.response = response_stream.str();
    return ;
}

ContentType HttpFileServing::createContentTypeMap( void )
{
    ContentType contentTypeMap;

    contentTypeMap[".html"] = "text/html";
    contentTypeMap[".jpg"] = "image/jpeg";
    contentTypeMap[".ico"] = "image/x-icon";
    contentTypeMap[".png"] = "image/png";
    contentTypeMap[".mp4"] = "video/mp4";
    contentTypeMap[".css"] = "text/css";
    contentTypeMap[".js"] = "text/javascript";
    contentTypeMap[".txt"] = "text/plain";
    contentTypeMap[".pdf"] = "application/pdf";
    contentTypeMap[".zip"] = "application/zip";
    contentTypeMap[".gz"] = "application/gzip";
    contentTypeMap[".tar"] = "application/x-tar";
    contentTypeMap[".xml"] = "application/xml";
    contentTypeMap[".json"] = "application/json";
    contentTypeMap[".svg"] = "image/svg+xml";
    contentTypeMap[".mp3"] = "audio/mpeg";
    contentTypeMap[".wav"] = "audio/wav";
    contentTypeMap[".ogg"] = "audio/ogg";
    contentTypeMap[".webm"] = "video/webm";
    contentTypeMap[".webp"] = "image/webp";
    // Add more content types here

    return ( contentTypeMap );
}

// Function to determine Content-Type based on file extension
std::string HttpFileServing::getContentType( const std::string & fullPath )
{
    static ContentType contentTypeMap = createContentTypeMap();

    std::string contentType = "text/plain";

    std::string::size_type pos = fullPath.find_last_of( "." );
    if ( pos != std::string::npos )
    {
        std::string ext = fullPath.substr( pos );
        ContentType::const_iterator it = contentTypeMap.find( ext );
        if ( it != contentTypeMap.end() )
            contentType = it->second;
    }

    return ( contentType );
}