/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpFileServing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:34:05 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/28 17:26:03 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

static  std::string getContentType( const std::string & fullPath );

void    Http::httpFileServing1( std::string path, std::string fullPath, Data & data, ConfigData & config )
{
    LOG(INFO) << ft::prettyPrint(__FUNCTION__, __LINE__, "Serving file");

    std::string filename = path.substr(path.find_last_of( "/" ) + 1 );
    std::string::size_type dotPos = filename.find_last_of( "." );
    if ( dotPos != std::string::npos )
    {
        std::string statusCodeStr = filename.substr( 0, dotPos );
        int status_code;
        std::istringstream( statusCodeStr ) >> status_code;

        if ( status_code )
            return ( HttpErrors::sendError( data, status_code, config ) );
    }

    std::fstream * file = ft::openFile( fullPath, std::ios::in | std::ios::binary );
    if ( !file || !file->is_open() )
        return ( HttpErrors::sendError( data, NOT_FOUND, config ) );
    
    std::string content = std::string( ( std::istreambuf_iterator< char >( *file ) ), std::istreambuf_iterator< char >() );


    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 200 OK\r\n";
    response_stream << "Content-Length: " << content.size() << "\r\n";
    response_stream << "Content-Type: " << getContentType( fullPath ) << "\r\n";
    response_stream << "\r\n";
    response_stream << content;

    // Store response in data
    data.response = response_stream.str();
    return ;
}

static  ContentType createContentTypeMap( void )
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
static  std::string getContentType( const std::string & fullPath )
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