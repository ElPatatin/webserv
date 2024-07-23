/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethods.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:01:26 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 15:38:59 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHODS_HPP
# define HTTPMETHODS_HPP

# include "common.hpp"
# include "Log.hpp"
# include "Exceptions.hpp"

# include <map>

class HttpMethods
{
    public:
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
        
        typedef std::map< std::string, Method > Methods;

        static std::string toString( Method method );
        static Method fromString( const std::string & method );

    protected:
        HttpMethods( void );
        HttpMethods( const HttpMethods & src );
        HttpMethods & operator=( const HttpMethods & rhs );
        ~HttpMethods( void );

    private:
        static Methods createMethodMap( void );

        static Methods methodMap;

};


#endif