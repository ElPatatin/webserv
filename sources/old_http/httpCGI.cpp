/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:32:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 16:14:49 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sys/wait.h>
#include <unistd.h>

void Http::executeCGI( const std::string & scriptPath, const std::string & queryString, std::ostringstream & response_stream, Data & data, ConfigData & config, HttpData & http )
{

    int pipe_fd[ 2 ];
    if ( pipe( pipe_fd ) == -1 )
        return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    int pid = fork();
    if ( pid == -1 )
        return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    if ( pid == 0 )
    {
        if ( close( pipe_fd[ 0 ] ) == -1 || dup2( pipe_fd[ 1 ], STDOUT_FILENO ) == -1 || close( pipe_fd[ 1 ] ) == -1 )
            std::exit(EXIT_FAILURE);

        std::string query = "QUERY_STRING=" + queryString;
        std::string contentLength = "CONTENT_LENGTH=" + http.headers[ "Content-Length" ].second;
        std::string contentType = "CONTENT_TYPE=" + http.headers[ "Content-Type" ].second;

        char *argv[] = { (char *)scriptPath.c_str(), NULL };
        char *envp[] = { (char *)query.c_str(), (char *)contentLength.c_str(), (char *)contentType.c_str(), NULL };


        execve( scriptPath.c_str(), argv, envp );
        std::exit( EXIT_FAILURE );
    }
    else
    {

        if ( close( pipe_fd[ 1 ] ) == -1)
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        char buffer[ 4096 ];
        int bytesRead;
        while ( ( bytesRead = read( pipe_fd[ 0 ], buffer, sizeof( buffer ) ) ) > 0 )
            response_stream.write( buffer, bytesRead );

        if ( bytesRead == -1 )
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        if ( close( pipe_fd[ 0 ]) == -1 )
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        int status;
        if ( waitpid( pid, &status, 0 ) == -1 )
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        if ( WIFEXITED( status ) && WEXITSTATUS( status ) != 0 )
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

        if ( response_stream.str().empty() )
        {
            return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );
        }
        else
        {
            std::ostringstream header_stream;
            header_stream << "HTTP/1.1 200 OK\r\n"
                        << "Content-Length: " << response_stream.str().length() << "\r\n"
                        << "Content-Type: text/html\r\n"
                        << "\r\n"
                        << response_stream.str();
            data.response = header_stream.str();
        }
    }

    return ;
}