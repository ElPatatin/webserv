/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 12:53:01 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/02 12:53:00 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include "common.hpp"
# include "Log.hpp"
# include "utils.hpp"
# include "Exceptions.hpp"
# include "webserver.hpp"

# include <map>

typedef std::map<std::string, std::string> Headers;

namespace Http
{
    typedef struct s_http
    {
        std::string method;
        std::string path;
        std::string version;
        std::string body;
        Headers     headers;
    }   HttpData;

    void    httpRequest( HttpData & http, Data & data, ConfigData config );
    void    sendError( Data & data, int status_code, ConfigData config );
}

#endif