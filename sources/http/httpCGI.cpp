/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramos-m <pramos-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:32:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 17:03:35 by pramos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <map>
#include <fstream>

/**
 * @brief Execute a CGI script and send its output as the response
 * 
 * @param scriptPath Path to the CGI script
 * @param queryString Query string to be passed to the CGI script
 * @param response_stream Stream to store the response from the CGI script
 * @param data Reference to the Data structure
 * @param config Reference to the ConfigData structure
 * @param http Reference to the HttpData structure
 */
void    Http::executeCGI( const std::string &scriptPath, const std::string &queryString, std::ostringstream &response_stream, Data &data, ConfigData &config, HttpData &http )
{
    (void)http; // Suppress unused parameter warning

    // Temporary file for CGI output
    std::string tmpFilePath = "/tmp/cgi_output.txt";
    std::string command = scriptPath + " > " + tmpFilePath;

    // Append query string if not empty
    if  ( !queryString.empty() )
        command += " " + queryString;

    // Execute the command
    int result = std::system( command.c_str() );
    if  ( result == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to execute CGI script using std::system" );
        return HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config );
    }

    // Open the temporary file to read the CGI output
    std::ifstream tmpFile( tmpFilePath.c_str() );
    if  ( !tmpFile.is_open() )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to open temporary file for CGI output" );
        return HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config );
    }

    // Read the content of the temporary file line by line and add to the response stream
    std::string line;
    while ( std::getline( tmpFile, line ) )
        response_stream << line << "\n";
    tmpFile.close(); // Close the temporary file

    // Save the response in the data structure
    data.response = response_stream.str();
    LOG( DEBUG ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Response from CGI script: " + data.response );
}