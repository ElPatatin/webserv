/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDirListing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 19:15:11 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/29 19:06:03 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpFileServing.hpp"

void    HttpFileServing::httpDirectoryListing( Data & data, const ConfigData & config, const HttpRequestParser::Request & request, const std::string & full_path )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Serving directory listing" );

    DIR *dir = opendir( full_path.c_str() );
    if ( !dir )
        return ( HttpFileServing::httpErrorServing( data, request, NOT_FOUND, config ) );

    std::ostringstream response;
    Location location = config.getLocation( "/" );
    std::string root;
    for ( size_t i = 0; i < location.size(); ++i )
    {
        if ( location[ i ].first == "root" )
            root = location[ i ].second;
    }

    HttpFileServing::generateDirectoryPage( response, dir, request.url, full_path, root );

    if ( closedir( dir ) == -1 )
        return ( HttpFileServing::httpErrorServing( data, request, INTERNAL_SERVER_ERROR, config ) );

    std::string content = response.str();
    
    std::ostringstream response_stream;

    response_stream << HttpVersion::toString( request.version ) << " " << OK << " " << HttpResponse::toString( OK ) << "\r\n";
    response_stream << "Content-Length: " << content.size() << "\r\n";
    response_stream << "Content-Type: " << "text/html" << "\r\n";
    response_stream << "\r\n";
    response_stream << content;

    // Store response in data
    data.response = response_stream.str();
    return ;
}

void HttpFileServing::generateDirectoryPage( std::ostringstream & response_stream, DIR * dir, const std::string & path, const std::string & fullPath, const std::string & root )
{
    struct dirent *ent;
    struct stat st;
    std::vector< std::string > file_content;
    int line_number = 0;
    bool file_opened;


    std::fstream *file = ft::openFile( root + "/directory.html", std::ios::in | std::ios::binary );
    if ( !file || !file->is_open() )
    {
        file_opened = false;
        response_stream << "<!DOCTYPE html>"
                        << "<html lang=\"en\">"
                        << "<head>"
                        << "<meta charset=\"UTF-8\">"
                        << "<title>Directory Listing</title>"
                        << "</head>"
                        << "<body>"
                        << "<h1>Directory Listing</h1>"
                        << "<ul>";
    }
    else
    {
        file_opened = true;
        std::string line;
        while ( std::getline( *file, line ) )
            file_content.push_back( line );
        ft::closeFile( file );

        for ( std::vector< std::string >::iterator it = file_content.begin(); it != file_content.end(); it++ )
        {
            ++line_number;

            std::string line = *it;
            if ( line.find( "{$PATH}" ) != std::string::npos )
                line.replace( line.find( "{$PATH}" ), 7, path ); // Replace {$PATH} placeholder with current path
            else if ( line.find( "{$LIST}" ) != std::string::npos )
                break;
            response_stream << line << std::endl;
        }
    }

    // Add back directory link
    if ( path != "/" )
    {
        std::string parentPath = path.substr( 0, path.find_last_of( "/" ) );
        if ( parentPath.empty() )
            parentPath = "/";

        response_stream << "<li>"
                        << "<span class=\"icon icon-folder\"></span>"
                        << "<a href=\"" << parentPath << "\">Parent Directory</a>"
                        << "</li>";
    }

    while ( ( ent = readdir( dir ) ) != NULL )
    {
        std::string name = ent->d_name;
        if ( name != "." && name != ".." )
        {
            std::string filePath = fullPath + "/" + name;
            stat( filePath.c_str(), &st );
            if ( S_ISDIR( st.st_mode ) )
            {
                response_stream << "<li>"
                                << "<span class=\"icon icon-folder\" onclick=\"toggle(event)\"></span>"
                                << "<a href=\"" << (path == "/" ? "" : path) << "/" << name << "\">" << name << "</a>"
                                << "<ul style=\"display: none;\"></ul>"
                                << "</li>";
            }
            else
            {
                response_stream << "<li>"
                                << "<span class=\"icon icon-file\"></span>"
                                << "<a href=\"" << (path == "/" ? "" : path) << "/" << name << "\">" << name << "</a>"
                                << "</li>";
            }
        }
    }

    if ( !file_opened )
    {
        // Close the unordered list and the body
        response_stream << "</ul>"
                        << "</body>"
                        << "</html>";
    }
    else
    {
        // Process remaining lines of the template
        for ( std::vector< std::string >::iterator it = file_content.begin() + line_number; it != file_content.end(); it++ )
        {
            if ( it->find( "{$LIST}" ) != std::string::npos )
                continue ;
            response_stream << *it << std::endl;
        }
    }

    return ;
}