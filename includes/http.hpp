/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 12:53:01 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/04 16:03:45 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include "common.hpp"
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
    std::string version;
    std::string body;
    Headers     headers;

}   HttpData;

namespace Http
{
    void    httpRequest( HttpData & http, Data & data, ConfigData config );
}

namespace HttpMethods
{
    std::string toString( Method method );
    Method methodFromString( const std::string& method );
}

namespace HttpErrors
{
    void    sendError( Data & data, int status_code, ConfigData config );
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

#endif