/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pramos-m <pramos-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:32:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/21 16:11:46 by pramos-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <map>
#include <fstream>

void Http::executeCGI(const std::string &scriptPath, const std::string &queryString, std::ostringstream &response_stream, Data &data, ConfigData &config, HttpData &http)
{
    (void)queryString; // Suppress unused parameter warning
    (void)http; // Suppress unused parameter warning

    // 1.path 2. httpresponse(argument y redireccion a archivo, que todos los prints de py van a stdout y en consecuencia al file.)
    // En el py, con py y su libreria cgi hay que usar la libreria para parsear el http (lo hace solo)
    // Despues hay que devolver la response del gci. 
    
    std::string tmpFilePath = "/tmp/cgi_output.txt"; // Archivo temporal para la salida del CGI
    std::string command = scriptPath + " > " + tmpFilePath; // Comando para ejecutar el script y redirigir la salida al archivo temporal

    // Ejecutar el comando
    int result = std::system(command.c_str());
    if (result == -1)
    {
        LOG(ERROR) << ft::prettyPrint(__FUNCTION__, __LINE__, "Failed to execute CGI script using std::system");
        return HttpErrors::sendError(data, INTERNAL_SERVER_ERROR, config);
    }

    // Abrir el archivo temporal para leer la salida del script CGI
    std::ifstream tmpFile(tmpFilePath.c_str()); // Cambiado a .c_str() para convertir std::string a const char*
    if (!tmpFile.is_open())
    {
        LOG(ERROR) << ft::prettyPrint(__FUNCTION__, __LINE__, "Failed to open temporary file for CGI output");
        return HttpErrors::sendError(data, INTERNAL_SERVER_ERROR, config);
    }

    // Leer el contenido del archivo temporal línea por línea y añadirlo al flujo de respuesta
    std::string line;
    while (std::getline(tmpFile, line))
    {
        response_stream << line << "\n";
    }
    tmpFile.close(); // Cerrar el archivo temporal

    data.response = response_stream.str(); // Guardar la respuesta en la estructura de datos
    LOG(DEBUG) << ft::prettyPrint(__FUNCTION__, __LINE__, "Response from CGI script: " + data.response);
}

        
   // en vez de ls poner el path del script
   
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

