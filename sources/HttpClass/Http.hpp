/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:22:07 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 20:08:25 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include "HttpRequestParser.hpp"
# include "HttpFileServing.hpp"
# include "ConfigData.hpp"
# include "structs.hpp"
# include "utils.hpp"

class Http : public HttpRequestParser
{
    public:
        typedef struct http_s
        {
            std::string full_path;
            std::string root;
            Location    location;
            bool        autoindex;
            std::string index_page;
            std::string endpoint;
            bool        dir_list;
        }   Http_t;

        static Http::Request parseRequest( const std::string & request );
        static void handleRequest( const std::string & request, const ConfigData & config_data, const Data & data );

    private:
        Http( void );
        Http( const Http & src );
        Http & operator=( const Http & rhs );
        ~Http( void );

        static Http::Http_t getHttpData( const Http::Request & request, const ConfigData & config_data );
        static bool         handleRedirect( const Request & request, const ConfigData & config_data, const Data & data );
        static void         handleCGI( const Request & request, const ConfigData & config_data, const Data & data, std::string & full_url );
        static void         handleMethod( const Request & request, const ConfigData & config_data, const Data & data, const Http::Http_t & http_data );

        static void         handleGet( const Request & request, const ConfigData & config_data, const Data & data, const Http::Http_t & http_data );
        static void         handleDelete( const Request & request, const ConfigData & config_data, const Data & data, const Http::Http_t & http_data );
        static void         handlePost( const Request & request, const ConfigData & config_data, const Data & data, const Http::Http_t & http_data );

        static std::string  getFullUrl( const std::string & root, const std::string & url );
        static std::string  getEndpoint( const std::string & url );
        static std::string  locationFinder( const Location & location, const std::string & key );
        static bool         checkErrorFile( const std::string & full_path, ErrorPages & error_pages, int * response_code );
        static std::pair< unsigned short, std::string > getRedirect( const std::string & endpoint, const Redirects & redirects );
};

#endif