/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpFileServing.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:01:06 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/28 19:54:59 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPFILESERVING_HPP
# define HTTPFILESERVING_HPP

# include "utils.hpp"
# include "ConfigData.hpp"
# include "Log.hpp"
# include "Exceptions.hpp"
# include "structs.hpp"
# include "httpResponseCodes.hpp"
# include "HttpRequestParser.hpp"
# include <dirent.h>

typedef std::map< std::string, std::string >      ContentType;

class HttpFileServing
{
    public:
        static void httpFileServing( Data & data, const ConfigData & config, const HttpRequestParser::Request & Request, const int & response_code, const std::string & full_path );
        static void httpErrorServing( Data & data, const HttpRequestParser::Request & request, const int & response_code, const ConfigData & config );
        static void httpDirectoryListing( Data & data, const ConfigData & config, const HttpRequestParser::Request & request, const std::string & full_path );

    private:
                    HttpFileServing( void );
                    HttpFileServing( const HttpFileServing & src );
                    HttpFileServing & operator=( const HttpFileServing & rhs );
                    ~HttpFileServing( void );

        static void httpDataServing( Data & data, const HttpRequestParser::Request & Request, const int & response_code, const std::string & content );
        static void generateDirectoryPage( std::ostringstream & response_stream, DIR * dir, const std::string & path, const std::string & fullPath, const std::string & root );

        static std::string getContentType( const std::string & fullPath );
        static ContentType createContentTypeMap( void );
};

#endif