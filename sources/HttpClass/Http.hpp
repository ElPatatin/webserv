/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:22:07 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/28 19:18:55 by cpeset-c         ###   ########.fr       */
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
        static Http::Request parseRequest( const std::string & request );
        static void handleRequest( const std::string & request, const ConfigData & config_data, const Data & data );
        static bool handleCGI( const std::string & request, const ConfigData & config_data, const Data & data );

    private:
        Http( void );
        Http( const Http & src );
        Http & operator=( const Http & rhs );
        ~Http( void );

        static std::string getFullUrl( const std::string & root, const std::string & url );
        static std::string getEndpoint( const std::string & url );
        static std::string locationFinder( const Location & location, const std::string & key );
        
};

#endif