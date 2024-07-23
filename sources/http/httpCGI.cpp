/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpCGI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:32:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 16:52:03 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <map>
#include <fstream>

/**
 * @brief Execute a CGI script and send its output as the response
*/
void Http::executeCGI(const std::string &scriptPath, const std::string &queryString, std::ostringstream &response_stream, Data &data, ConfigData &config, HttpData &http)
{
    (void)http; // Suppress unused parameter warning

    // 1.path 2. httpresponse(argument y redireccion a archivo, que todos los prints de py van a stdout y en consecuencia al file.)
    // En el py, con py y su libreria cgi hay que usar la libreria para parsear el http (lo hace solo)
    // Despues hay que devolver la response del gci. 
    
    std::string tmpFilePath = "/tmp/cgi_output.txt"; // Archivo temporal para la salida del CGI
    std::string command = scriptPath + " > " + tmpFilePath; // Comando para ejecutar el script y redirigir la salida al archivo temporal

    // Ejecutar el comando
    if (!queryString.empty())
        command += " " + queryString;

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
        response_stream << line << "\n";
    tmpFile.close(); // Cerrar el archivo temporal

    data.response = response_stream.str(); // Guardar la respuesta en la estructura de datos
    LOG(DEBUG) << ft::prettyPrint(__FUNCTION__, __LINE__, "Response from CGI script: " + data.response);
}
