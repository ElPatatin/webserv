/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpMethods.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:07:41 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/02 18:46:11 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

std::string Methods::toString( Method method )
{
    switch ( method )
    {
        case Method::GET: return "GET";
        case Method::HEAD: return "HEAD";
        case Method::POST: return "POST";
        case Method::PUT: return "PUT";
        case Method::DELETE: return "DELETE";
        case Method::TRACE: return "TRACE";
        case Method::OPTIONS: return "OPTIONS";
        case Method::CONNECT: return "CONNECT";
        case Method::PATCH: return "PATCH";
        default: throw MethodNotAllowedException( "Method to string not allowed" );
    }
}

Method Methods::methodFromString( const std::string & method )
{
    static const std::pair<std::string, Method> method_pairs[] = {
        std::make_pair("GET", Method::GET),
        std::make_pair("HEAD", Method::HEAD),
        std::make_pair("POST", Method::POST),
        std::make_pair("PUT", Method::PUT),
        std::make_pair("DELETE", Method::DELETE),
        std::make_pair("TRACE", Method::TRACE),
        std::make_pair("OPTIONS", Method::OPTIONS),
        std::make_pair("CONNECT", Method::CONNECT),
        std::make_pair("PATCH", Method::PATCH)
    };

    static const std::map<std::string, Method> method_map(
        method_pairs, method_pairs + sizeof(method_pairs) / sizeof(method_pairs[0])
    );

    std::map<std::string, Method>::const_iterator it = method_map.find(method);
    if (it != method_map.end())
        return it->second;

    throw MethodNotAllowedException( "String to method not allowed" );
}