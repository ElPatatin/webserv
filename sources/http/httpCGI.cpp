/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramos-m <pramos-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:32:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/19 14:56:00 by pramos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <map>
char **createCGIEnvironment(const std::string &scriptPath, const std::string &queryString, Data &data, ConfigData &config, HttpData &http);
void Http::executeCGI( const std::string & scriptPath, const std::string & queryString, std::ostringstream & response_stream, Data & data, ConfigData & config, HttpData & http )
{
    std::system("ls -l >test.txt");// en vez de ls poner todo el http literal
    // LOG( INFO ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Executing CGI script" );

    // int pipe_fd[ 2 ];
    // if ( pipe( pipe_fd ) == -1 )
    //     return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    // int pid = fork();
    // if ( pid == -1 )
    //     return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    // if ( pid == 0 )
    // {
    //     if ( close( pipe_fd[ 0 ] ) == -1 || dup2( pipe_fd[ 1 ], STDOUT_FILENO ) == -1 || close( pipe_fd[ 1 ] ) == -1 )
    //         std::exit(EXIT_FAILURE);
        
    //     char **envp = createCGIEnvironment(scriptPath, queryString, data, config, http);
    //     char *argv[] = {(char *)scriptPath.c_str(), NULL};
        
    //     LOG(DEBUG) << ft::prettyPrint(__FUNCTION__, __LINE__, "Executing CGI script: " + scriptPath);

    //     execve(scriptPath.c_str(), argv, envp);
    //     LOG(ERROR) << ft::prettyPrint(__FUNCTION__, __LINE__, "Failed to execute CGI script");
    //     std::exit(EXIT_FAILURE);
    // }
    // else
    // {

    //     if ( close( pipe_fd[ 1 ] ) == -1)
    //         return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    //     char buffer[ 4096 ];
    //     int bytesRead;
    //     while ( ( bytesRead = read( pipe_fd[ 0 ], buffer, sizeof( buffer ) ) ) > 0 )
    //         response_stream.write( buffer, bytesRead );
    //     LOG( DEBUG ) << ft::prettyPrint( __FUNCTION__, __LINE__, "Response from CGI script: " + response_stream.str() );

    //     if ( bytesRead == -1 )
    //         return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    //     if ( close( pipe_fd[ 0 ]) == -1 )
    //         return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    //     int status;
    //     if ( waitpid( pid, &status, 0 ) == -1 )
    //         return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    //     if ( WIFEXITED( status ) && WEXITSTATUS( status ) != 0 )
    //         return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );

    //     if ( response_stream.str().empty() )
    //     {
    //         LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "No response from CGI script" );
    //         return ( HttpErrors::sendError( data, INTERNAL_SERVER_ERROR, config ) );
    //     }
    //     else
    //         data.response = response_stream.str();
    // }
    // return ;
}

char **createCGIEnvironment(const std::string &scriptPath, const std::string &queryString, Data &data, ConfigData &config, HttpData &http) {
    // std::string query = "QUERY_STRING=" + queryString;
    // std::string contentLength = "CONTENT_LENGTH=" + http.headers["Content-Length"].second;
    // std::string contentType = "CONTENT_TYPE=" + http.headers["Content-Type"].second;
    // std::string serverSoftware = NULL;
    // std::string serverName = NULL;
    // std::string gatewayInterface = "GATEWAY_INTERFACE=CGI /1.1";
    // std::string serverProtocol = "SERVER_PROTOCOL=HTTP /1.1";
    // std::string serverPort = "SERVER_PORT=" + std::to_string(config.getPort());
    // std::string requestMethod = "REQUEST_METHOD=" + http.method;
    // std::string scriptName = "SCRIPT_NAME=" + scriptPath;
    // std::string pathInfo = NULL;
    // std::string pathTranslated = NULL;
    // std::string remoteAddr = NULL;
    // std::string remoteHost = NULL;

    std::string query = "QUERY_STRING=" + queryString;
    std::string contentLength = "CONTENT_LENGTH=" + http.headers["Content-Length"].second;
    std::string contentType = "CONTENT_TYPE=" + http.headers["Content-Type"].second;
    std::string serverSoftware = NULL;
    std::string serverName = NULL;
    std::string gatewayInterface = "GATEWAY_INTERFACE=CGI /1.1";
    std::string serverProtocol = "SERVER_PROTOCOL=HTTP /1.1";
    std::string serverPort = "SERVER_PORT=" + std::to_string(config.getPort());
    std::string requestMethod = "REQUEST_METHOD=" + http.method;
    std::string scriptName = "SCRIPT_NAME=" + scriptPath;
    std::string pathInfo = NULL;
    std::string pathTranslated = NULL;
    std::string remoteAddr = NULL;
    std::string remoteHost = NULL;

    // std::string env = query + " " + contentLength + " " .....

    // char **envp = new char *[16];
    // envp[0] = strdup(query.c_str());
    // envp[1] = strdup(contentLength.c_str());
    // envp[2] = strdup(contentType.c_str());
    // envp[3] = strdup(serverSoftware.c_str());
    // envp[4] = strdup(serverName.c_str());
    // envp[5] = strdup(gatewayInterface.c_str());
    // envp[6] = strdup(serverProtocol.c_str());
    // envp[7] = strdup(serverPort.c_str());
    // envp[8] = strdup(requestMethod.c_str());
    // envp[9] = strdup(scriptName.c_str());
    // envp[10] = strdup(pathInfo.c_str());
    // envp[11] = strdup(pathTranslated.c_str());
    // envp[12] = strdup(remoteAddr.c_str());
    // envp[13] = strdup(remoteHost.c_str());
    // envp[14] = NULL;

    return envp;
}
