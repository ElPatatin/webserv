/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 12:53:01 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/20 17:33:20 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include "Log.hpp"
# include "utils.hpp"
# include "Exceptions.hpp"
# include "webserver.hpp"

# include "httpResponseCodes.hpp"

# include <map>

# define LINE_END "\r\n"
# define HTTP_VERSION "HTTP/1.1"

typedef std::pair< std::string, std::string >   Header;
typedef std::map< std::string, Header >         Headers;
typedef std::map< std::string, std::string >      ContentType;

typedef enum e_method
{
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    OPTIONS,
    TRACE,
    CONNECT,
    PATCH
}   Method;

typedef struct s_http
{
    Method      method;
    std::string path;
    std::string params;
    std::string version;
    std::string body;
    Headers     headers;
    bool        isCGI;

}   HttpData;

namespace Url
{
    std::string decode( const std::string & url );
    std::string encode( const std::string & url );
}

namespace Http
{
    void    httpRequest( HttpData & http, Data & data, ConfigData & config );
    void    httpDirectoryListing( std::string path, std::string fullPath, Data & data, ConfigData & config );
    void    httpFileServing( std::string path, std::string fullPath, Data & data, ConfigData & config );
    void    executeCGI(const std::string &scriptPath, const std::string &queryString, std::ostringstream &response_stream, Data &data, ConfigData &config, HttpData & http );
}

namespace HttpMethods
{
    std::string toString1( Method method );
    Method methodFromString1( const std::string& method );
}

namespace HttpErrors
{
    void    sendError( Data & data, int status_code, ConfigData & config );
}

namespace HttpHeaders
{
    std::string serializeHeader( Headers headers );
    Headers deserializeHeader( std::string header );
}

namespace HttpRequests
{
    HttpData    parseRequest( std::string buffer );
}

namespace HttpResponse
{
    std::string toString1( int status_code );
}

#endif