/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:10:24 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/05 18:08:37 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

#include <dirent.h> // For directory handling
#include <sstream>  // For string streams
#include <fstream>  // For file handling

void Http::httpRequest( HttpData &http, Data &data, ConfigData config )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Request received" );

    std::string method = HttpMethods::toString( http.method );
    std::string path = http.path == "/" ? "/index.html" : http.path;
    std::string fullPath = "./html" + path;

    if ( method == "GET" )
    {
        struct stat info;
        if ( stat( fullPath.c_str(), &info ) != 0 )
        {
            // Error accessing path
            HttpErrors::sendError(data, 404, config);
            return;
        }
        else if ( info.st_mode & S_IFDIR )
        {
            // Handle directory listing
            DIR *dir;
            struct dirent *ent;
            if ( ( dir = opendir( fullPath.c_str() ) ) != NULL )
            {
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
                closedir( dir );

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

                std::string response = header_stream.str();
                if ( send( data.new_fd, response.c_str(), response.length(), 0 ) == -1 )
                {
                    LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
                    throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
                }
            }
            else
               return HttpErrors::sendError( data, 404, config );
        }
        else
        {
            // Check if it is an error file
            if ( fullPath.find( "errors" ) != std::string::npos )
            {
                // Error file requested (e.g. 404.html)
                std::string filename = path.substr(path.find_last_of("/") + 1);
                std::string::size_type dotPos = filename.find_last_of(".");
                if (dotPos != std::string::npos)
                {
                    std::string statusCodeStr = filename.substr(0, dotPos);
                    int status_code;
                    std::istringstream(statusCodeStr) >> status_code;
                    
                    // Check if the conversion succeeded
                    if (!status_code && statusCodeStr != "0") {
                        // Handle conversion error here, possibly send a default error response
                        status_code = 500; // Internal Server Error or another appropriate error code
                    }

                    HttpErrors::sendError(data, status_code, config);
                }
                else
                {
                    // Handle case where filename format is unexpected
                    HttpErrors::sendError(data, 500, config); // Internal Server Error
                }
            }

            // Handle file serving
            std::ifstream file( fullPath.c_str(), std::ios::in | 
                ( fullPath.find( ".jpg" ) != std::string::npos || fullPath.find( ".ico" ) != std::string::npos ? std::ios::binary : std::ios::in ) );

            if ( !file.is_open( ) )
                return HttpErrors::sendError( data, 404, config );

            std::string content( ( std::istreambuf_iterator< char >( file ) ), std::istreambuf_iterator< char >() );
            file.close();

            std::ostringstream response_stream;
            response_stream << "HTTP/1.1 200 OK\r\n"
                            << "Content-Length: " << content.size() << "\r\n";

            if ( fullPath.find( ".html" ) != std::string::npos )
                response_stream << "Content-Type: text/html\r\n";
            else if ( fullPath.find( ".jpg" ) != std::string::npos )
                response_stream << "Content-Type: image/jpeg\r\n";
            else if ( fullPath.find( ".ico" ) != std::string::npos )
                response_stream << "Content-Type: image/x-icon\r\n";

            response_stream << "\r\n" << content;

            std::string response = response_stream.str();
            if ( send( data.new_fd, response.c_str(), response.length(), 0 ) == -1 )
            {
                LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "send: " + std::string( std::strerror( errno ) ) );
                throw SocketException( "Error: send: " + std::string( std::strerror( errno ) ) );
            }
        }
    }
    else
        HttpErrors::sendError( data, 405, config );
}