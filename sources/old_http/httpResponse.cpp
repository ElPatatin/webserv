/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 19:10:21 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/20 17:33:08 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"

std::string HttpResponse::toString1( int stauts_code )
{
    if ( stauts_code == 100 )
        return ( "Continue" );
    if ( stauts_code == 101 )
        return ( "Switching Protocols" );
    if ( stauts_code == 200 )
        return ( "OK" );
    if ( stauts_code == 201 )
        return ( "Created" );
    if ( stauts_code == 404 )
        return ( "Not Found" );
    if ( stauts_code == 405 )
        return ( "Method Not Allowed" );
    if ( stauts_code == 408 )
        return ( "Request Timeout" );
    if ( stauts_code == 413 )
        return ( "Payload Too Large" );
    if ( stauts_code == 414 )
        return ( "URI Too Long" );
    if ( stauts_code == 418 )
        return ( "I'm a teapot" );
    if ( stauts_code == 500 )
        return ( "Internal Server Error" );
    if ( stauts_code == 501 )
        return ( "Not Implemented" );
    return ( "Unknown" );
}