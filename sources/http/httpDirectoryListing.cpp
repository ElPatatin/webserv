/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpDirectoryListing.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:46:22 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/12 14:54:28 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

#include <dirent.h> // For directory handling
#include <sstream>  // For string streams
#include <fstream>  // For file handling
#include <sys/stat.h> // For file info


void generateDirectoryPage( std::ostringstream & response_stream, DIR * dir, const std::string & path, const std::string & fullPath );

void Http::httpDirectoryListing(std::string path, std::string fullPath, Data &data, ConfigData &config )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Directory listing requested" );

    if ( !config.getIsDirectoryListing() )
        return ( HttpErrors::sendError( data, FORBIDDEN, config ) );

    DIR *dir = opendir( fullPath.c_str() );
    if ( !dir )
        return ( HttpErrors::sendError( data, NOT_FOUND, config ) );

    std::ostringstream response_stream;
    generateDirectoryPage( response_stream, dir, path, fullPath );

    if ( closedir( dir ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "closedir: " + std::string( std::strerror( errno ) ) );
        throw std::runtime_error("Error: closedir: " + std::string( std::strerror( errno ) ) );
    }

    std::string content = response_stream.str();

    std::ostringstream header_stream;
    header_stream << "HTTP/1.1 200 OK\r\n"
                  << "Content-Length: " << content.length() << "\r\n"
                  << "Content-Type: text/html\r\n"
                  << "\r\n"
                  << content;

    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Sending directory listing response" );
    data.response = header_stream.str();
    return ;
}

void generateDirectoryPage( std::ostringstream & response_stream, DIR * dir, const std::string & path, const std::string & fullPath )
{
    struct dirent *ent;
    struct stat st;
    std::vector< std::string > file_content;
    int line_number = 0;
    bool file_opened;

    std::fstream *file = ft::openFile( "./html/directory.html", std::ios::in );
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