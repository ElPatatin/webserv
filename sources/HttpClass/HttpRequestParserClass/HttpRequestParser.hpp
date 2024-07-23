/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:26:44 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 17:25:25 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSER_HPP
# define HTTPREQUESTPARSER_HPP

# include "utils.hpp"
# include "HttpHeaders.hpp"
# include "HttpMethods.hpp"
# include "HttpResponse.hpp"
# include "HttpUrl.hpp"
# include "HttpVersion.hpp"

class HttpRequestParser : public HttpHeaders, public HttpMethods, public HttpResponse, public HttpUrl, public HttpVersion
{
    public:
        typedef struct s_request
        {
            Method      method;
            Version     version;
            std::string url;
            std::string query;
            bool        isCGI;
            Headers     headers;
            std::string body;
        }   Request;

        static Request parseRequest( const std::string & request );
        static std::string serializeRequest( const Request & request );
        static Request deserializedRequest( const std::string & request );

    private:
        HttpRequestParser( void );
        HttpRequestParser( const HttpRequestParser & src );
        HttpRequestParser & operator=( const HttpRequestParser & rhs );
        ~HttpRequestParser( void );

        static void parseUrl( Request & request, const std::string & line );
        static std::string parseQuery( const std::string & query );
        static bool parseIsCGI( const std::string & url );
        static void parseHeaders( Request & request_data, const std::string & request, size_t headerEndPos );
        static void parseBody( Request & request_data, const std::string & request, size_t headerEndPos );
        static void parseCookie( Request & request_data );

};

#endif