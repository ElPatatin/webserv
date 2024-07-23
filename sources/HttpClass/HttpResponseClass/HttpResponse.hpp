/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:01:53 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 11:35:25 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "common.hpp"
# include "httpResponseCodes.hpp"
# include "Log.hpp"
# include "Exceptions.hpp"

# include <map>

typedef std::map<std::string, int> StatusMessages;
typedef std::map<int, std::string> StatusCodes;

class HttpResponse
{
    public:
        static std::string toString( int status_code );
        static int fromString( const std::string & status_message );

    private:
        HttpResponse( void );
        HttpResponse( const HttpResponse& src );
        HttpResponse & operator=( const HttpResponse & rhs );
        ~HttpResponse( void );

        static StatusCodes     createStatusCodeMap( void );
        static StatusMessages  createStatusMessageMap( void );

        static StatusCodes     statusCodeMap;
        static StatusMessages  statusMessageMap;
};

#endif // HTTPRESPONSE_HPP