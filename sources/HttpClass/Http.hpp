/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:22:07 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 17:31:17 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include "HttpRequestParser.hpp"

class Http : public HttpRequestParser
{
    public:
        static void handleRequest( const std::string & request );

    private:
        Http( void );
        Http( const Http & src );
        Http & operator=( const Http & rhs );
        ~Http( void );

};

#endif