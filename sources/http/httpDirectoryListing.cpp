/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpDirectoryListing.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:46:22 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/09 16:09:06 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

#include <dirent.h> // For directory handling
#include <sstream>  // For string streams
#include <fstream>  // For file handling
#include <sys/stat.h> // For file info

void Http::httpDirectoryListing( std::string path, std::string fullPath, Data &data, ConfigData & config )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Directory listing requested" );

    if ( config.getIsDirectoryListing() == false )
        return ( HttpErrors::sendError( data, FORBIDDEN, config ) );

    // Handle directory listing
    DIR *dir;
    struct dirent *ent;
    dir = opendir( fullPath.c_str() );
    if ( !dir )
        return ( HttpErrors::sendError( data, NOT_FOUND, config ) );
        
    std::ostringstream response_stream;
    response_stream << "<!DOCTYPE html>"
                    << "<html lang=\"en\">"
                    << "<head>"
                    << "<meta charset=\"UTF-8\">"
                    << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                    << "<link rel=\"icon\" href=\"/favicon.ico\" type=\"image/x-icon\">"
                    << "<title>Directory Listing - Our Server</title>"
                    << "<style>"
                    << "body { margin: 0; font-family: 'Arial', sans-serif; background-color: #f5f5f5; color: #333; }"
                    << ".header { background-color: #4CAF50; color: white; padding: 10px 0; text-align: center; border-bottom: 5px solid #388E3C; }"
                    << ".nav { display: flex; justify-content: center; background-color: #333; border-bottom: 5px solid #222; }"
                    << ".nav a { color: white; padding: 14px 20px; text-decoration: none; text-align: center; border-right: 1px solid #555; }"
                    << ".nav a:hover { background-color: #ddd; color: black; }"
                    << ".content { padding: 20px; display: flex; justify-content: center; gap: 20px; flex-wrap: wrap; }"
                    << ".box { background-color: white; border: 1px solid #ddd; box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); padding: 20px; border-radius: 8px; flex: 1 1 300px; box-sizing: border-box; }"
                    << ".footer { background-color: #333; color: white; padding: 10px 0; text-align: center; position: fixed; width: 100%; bottom: 0; border-top: 5px solid #222; }"
                    << ".tree ul { list-style-type: none; }"
                    << ".tree ul ul { margin-left: 20px; padding-left: 20px; border-left: 1px dashed #ccc; }"
                    << ".tree li { margin: 5px 0; position: relative; }"
                    << ".tree li::before { content: ''; position: absolute; top: 0; left: -10px; width: 10px; height: 100%; border-left: 1px solid #ccc; border-bottom: 1px solid #ccc; }"
                    << ".tree li:last-child::before { height: 10px; border-bottom: none; }"
                    << ".tree li a { text-decoration: none; color: #1a73e8; }"
                    << ".tree li a:hover { text-decoration: underline; }"
                    << ".icon { margin-right: 5px; }"
                    << ".icon-folder::before { content: '\\1F4C1'; }" // Unicode for folder icon
                    << ".icon-file::before { content: '\\1F4C4'; }" // Unicode for file icon
                    << "</style>"
                    << "<script>"
                    << "function toggle(event) {"
                    << "  var nextUl = event.target.nextElementSibling;"
                    << "  if (nextUl && nextUl.tagName === 'UL') {"
                    << "    nextUl.style.display = nextUl.style.display === 'none' ? 'block' : 'none';"
                    << "  }"
                    << "}"
                    << "</script>"
                    << "</head>"
                    << "<body>"
                    << "<div class=\"header\">"
                    << "<h1>Directory Listing</h1>"
                    << "</div>"
                    << "<div class=\"nav\">"
                    << "<a href=\"/index.html\">Home</a>"
                    << "<a href=\"/about.html\">About</a>"
                    << "<a href=\"/contact.html\">Contact</a>"
                    << "<a href=\"/codes.html\">Errors</a>"
                    << "</div>"
                    << "<div class=\"content\">"
                    << "<div class=\"box\">"
                    << "<h2>Files in " << path << "</h2>"
                    << "<div class=\"tree\">"
                    << "<ul>";

    // Add back directory link
    if ( path != "/" )
    {
        std::string parentPath = path.substr( 0, path.find_last_of( "/" ) );
        if ( parentPath.empty() ) parentPath = "/";
        response_stream << "<li>"
                        << "<span class=\"icon icon-folder\"></span>"
                        << "<a href=\"" << parentPath << "\">..</a>"
                        << "</li>";
    }

    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Listing files in directory: " + fullPath );
    while ( ( ent = readdir( dir ) ) != NULL )
    {
        std::string name = ent->d_name;
        if ( name != "." && name != ".." )
        {
            struct stat st;
            std::string filePath = fullPath + "/" + name;
            stat( filePath.c_str(), &st );
            if ( S_ISDIR( st.st_mode) )
            {
                response_stream << "<li>"
                                << "<span class=\"icon icon-folder\" onclick=\"toggle(event)\"></span>"
                                << "<a href=\"" << path << "/" << name << "\">" << name << "</a>"
                                << "<ul style=\"display: none;\"></ul>" // Placeholder for future expansion
                                << "</li>";
            }
            else
            {
                response_stream << "<li>"
                                << "<span class=\"icon icon-file\"></span>"
                                << "<a href=\"" << path << "/" << name << "\">" << name << "</a>"
                                << "</li>";
            }
        }
    }

    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Closing directory" );
    if ( closedir( dir ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "closedir: " + std::string( std::strerror( errno ) ) );
        throw DirectoryListingException( "Error: closedir: " + std::string( std::strerror( errno ) ) );
    }

    response_stream << "</ul>"
                    << "</div>" // close tree
                    << "</div>" // close box
                    << "</div>" // close content
                    << "<div class=\"footer\">"
                    << "<p>&copy; 2024 Our Web Server. All rights reserved.</p>"
                    << "</div>"
                    << "</body>"
                    << "</html>";

    std::string content = response_stream.str();

    std::ostringstream header_stream;
    header_stream << "HTTP/1.1 200 OK\r\n"
                << "Content-Length: " << content.length() << "\r\n"
                << "Content-Type: text/html\r\n"
                << "\r\n"
                << content;

    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Sending directory listing response" );
    data.response = header_stream.str();
}