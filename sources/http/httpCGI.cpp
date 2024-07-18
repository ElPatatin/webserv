/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramos-m <pramos-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:32:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/18 15:47:13 by pramos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sys/wait.h>
#include <unistd.h>

void Http::executeCGI( const std::string & scriptPath, const std::string & queryString, std::ostringstream & response_stream, Data & data, ConfigData & config, HttpData & http )
{
    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Executing CGI script" );

    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Creating pipe" );
    int pipe_fd[ 2 ];
    if ( pipe( pipe_fd ) == -1 )
        return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Forking process" );
    int pid = fork();
    if ( pid == -1 )
        return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Child process" );
    if ( pid == 0 )
    {
        if ( close( pipe_fd[ 0 ] ) == -1 || dup2( pipe_fd[ 1 ], STDOUT_FILENO ) == -1 || close( pipe_fd[ 1 ] ) == -1 )
            std::exit(EXIT_FAILURE);

        LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Setting environment variables" );
        std::string query = "QUERY_STRING=" + queryString;
        std::string contentLength = "CONTENT_LENGTH=" + http.headers[ "Content-Length" ].second;
        std::string contentType = "CONTENT_TYPE=" + http.headers[ "Content-Type" ].second;

        LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Executing script" );
        char *argv[] = { (char *)scriptPath.c_str(), NULL };
        char *envp[] = { (char *)query.c_str(), (char *)contentLength.c_str(), (char *)contentType.c_str(), NULL };

        LOG( DEBUG ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Executing CGI script: " + scriptPath );
        LOG( DEBUG ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Arguments: " + query + ", " + contentLength + ", " + contentType );

        execve( scriptPath.c_str(), argv, envp );
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Failed to execute CGI script" );
        std::exit( EXIT_FAILURE );
    }
    else
    {
        LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Parent process" );

        if ( close( pipe_fd[ 1 ] ) == -1)
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Reading from pipe" );
        char buffer[ 4096 ];
        int bytesRead;
        while ( ( bytesRead = read( pipe_fd[ 0 ], buffer, sizeof( buffer ) ) ) > 0 )
            response_stream.write( buffer, bytesRead );
        LOG( DEBUG ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Response from CGI script: " + response_stream.str() );

        if ( bytesRead == -1 )
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        if ( close( pipe_fd[ 0 ]) == -1 )
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Waiting for child process" );
        int status;
        if ( waitpid( pid, &status, 0 ) == -1 )
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Checking child process status" );
        if ( WIFEXITED( status ) && WEXITSTATUS( status ) != 0 )
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        if ( response_stream.str().empty() )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "No response from CGI script" );
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );
        }
        else
            data.response = response_stream.str();
    }
    return ;
}