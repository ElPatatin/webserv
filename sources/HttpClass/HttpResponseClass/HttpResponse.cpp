/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:01:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 11:32:13 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse( void )
{
    this->statusCodeMap = this->createStatusCodeMap();
    this->statusMessageMap = this->createStatusMessageMap();

    return ;
}

HttpResponse::HttpResponse( const HttpResponse& src )
{
    *this = src;

    return ;
}

HttpResponse & HttpResponse::operator=( const HttpResponse & rhs )
{
    if (this != &rhs)
    {
        this->statusCodeMap = rhs.statusCodeMap;
        this->statusMessageMap = rhs.statusMessageMap;
    }

    return ( *this );
}

HttpResponse::~HttpResponse( void ) { return ; }

std::string HttpResponse::toString( int status_code )
{
    StatusCodes::const_iterator it = statusCodeMap.find( status_code );

    if ( it != statusCodeMap.end() )
        return ( it->second );
    return ( NULL );
}

int HttpResponse::fromString( const std::string & status_message )
{
    StatusMessages::const_iterator it = statusMessageMap.find( status_message );

    if ( it != statusMessageMap.end() )
        return ( it->second );
    return ( -1 );
}


StatusCodes HttpResponse::createStatusCodeMap( void )
{
    StatusCodes map;

    map[CONTINUE] = "Continue";
    map[SWITCHING_PROTOCOLS] = "Switching Protocols";
    map[PROCESSING] = "Processing";
    map[EARLY_HINTS] = "Early Hints";
    map[OK] = "OK";
    map[CREATED] = "Created";
    map[ACCEPTED] = "Accepted";
    map[NON_AUTHORITATIVE_INFORMATION] = "Non-Authoritative Information";
    map[NO_CONTENT] = "No Content";
    map[RESET_CONTENT] = "Reset Content";
    map[PARTIAL_CONTENT] = "Partial Content";
    map[MULTI_STATUS] = "Multi-Status";
    map[ALREADY_REPORTED] = "Already Reported";
    map[IM_USED] = "IM Used";
    map[MULTIPLE_CHOICES] = "Multiple Choices";
    map[MOVED_PERMANENTLY] = "Moved Permanently";
    map[FOUND] = "Found";
    map[SEE_OTHER] = "See Other";
    map[NOT_MODIFIED] = "Not Modified";
    map[USE_PROXY] = "Use Proxy";
    map[SWITCH_PROXY] = "Switch Proxy"; // Unused
    map[TEMPORARY_REDIRECT] = "Temporary Redirect";
    map[PERMANENT_REDIRECT] = "Permanent Redirect";
    map[BAD_REQUEST] = "Bad Request";
    map[UNAUTHORIZED] = "Unauthorized";
    map[PAYMENT_REQUIRED] = "Payment Required";
    map[FORBIDDEN] = "Forbidden";
    map[NOT_FOUND] = "Not Found";
    map[METHOD_NOT_ALLOWED] = "Method Not Allowed";
    map[NOT_ACCEPTABLE] = "Not Acceptable";
    map[PROXY_AUTHENTICATION_REQUIRED] = "Proxy Authentication Required";
    map[REQUEST_TIMEOUT] = "Request Timeout";
    map[CONFLICT] = "Conflict";
    map[GONE] = "Gone";
    map[LENGTH_REQUIRED] = "Length Required";
    map[PRECONDITION_FAILED] = "Precondition Failed";
    map[PAYLOAD_TOO_LARGE] = "Payload Too Large";
    map[URI_TOO_LONG] = "URI Too Long";
    map[UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Type";
    map[RANGE_NOT_SATISFIABLE] = "Range Not Satisfiable";
    map[EXPECTATION_FAILED] = "Expectation Failed";
    map[IM_A_TEAPOT] = "I'm a teapot";
    map[MISDIRECTED_REQUEST] = "Misdirected Request";
    map[UNPROCESSABLE_ENTITY] = "Unprocessable Entity";
    map[LOCKED] = "Locked";
    map[FAILED_DEPENDENCY] = "Failed Dependency";
    map[TOO_EARLY] = "Too Early";
    map[UPGRADE_REQUIRED] = "Upgrade Required";
    map[PRECONDITION_REQUIRED] = "Precondition Required";
    map[TOO_MANY_REQUESTS] = "Too Many Requests";
    map[REQUEST_HEADER_FIELDS_TOO_LARGE] = "Request Header Fields Too Large";
    map[UNAVAILABLE_FOR_LEGAL_REASONS] = "Unavailable For Legal Reasons";
    map[INTERNAL_SERVER_ERROR] = "Internal Server Error";
    map[NOT_IMPLEMENTED] = "Not Implemented";
    map[BAD_GATEWAY] = "Bad Gateway";
    map[SERVICE_UNAVAILABLE] = "Service Unavailable";
    map[GATEWAY_TIMEOUT] = "Gateway Timeout";
    map[HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
    map[VARIANT_ALSO_NEGOTIATES] = "Variant Also Negotiates";
    map[INSUFFICIENT_STORAGE] = "Insufficient Storage";
    map[LOOP_DETECTED] = "Loop Detected";
    map[NOT_EXTENDED] = "Not Extended";
    map[NETWORK_AUTHENTICATION_REQUIRED] = "Network Authentication Required";

    return ( map );
}

StatusMessages  HttpResponse::createStatusMessageMap( void )
{
    StatusMessages  map;

    map["Continue"] = CONTINUE;
    map["Switching Protocols"] = SWITCHING_PROTOCOLS;
    map["Processing"] = PROCESSING;
    map["Early Hints"] = EARLY_HINTS;
    map["OK"] = OK;
    map["Created"] = CREATED;
    map["Accepted"] = ACCEPTED;
    map["Non-Authoritative Information"] = NON_AUTHORITATIVE_INFORMATION;
    map["No Content"] = NO_CONTENT;
    map["Reset Content"] = RESET_CONTENT;
    map["Partial Content"] = PARTIAL_CONTENT;
    map["Multi-Status"] = MULTI_STATUS;
    map["Already Reported"] = ALREADY_REPORTED;
    map["IM Used"] = IM_USED;
    map["Multiple Choices"] = MULTIPLE_CHOICES;
    map["Moved Permanently"] = MOVED_PERMANENTLY;
    map["Found"] = FOUND;
    map["See Other"] = SEE_OTHER;
    map["Not Modified"] = NOT_MODIFIED;
    map["Use Proxy"] = USE_PROXY;
    map["Switch Proxy"] = SWITCH_PROXY; // Unused
    map["Temporary Redirect"] = TEMPORARY_REDIRECT;
    map["Permanent Redirect"] = PERMANENT_REDIRECT;
    map["Bad Request"] = BAD_REQUEST;
    map["Unauthorized"] = UNAUTHORIZED;
    map["Payment Required"] = PAYMENT_REQUIRED;
    map["Forbidden"] = FORBIDDEN;
    map["Not Found"] = NOT_FOUND;
    map["Method Not Allowed"] = METHOD_NOT_ALLOWED;
    map["Not Acceptable"] = NOT_ACCEPTABLE;
    map["Proxy Authentication Required"] = PROXY_AUTHENTICATION_REQUIRED;
    map["Request Timeout"] = REQUEST_TIMEOUT;
    map["Conflict"] = CONFLICT;
    map["Gone"] = GONE;
    map["Length Required"] = LENGTH_REQUIRED;
    map["Precondition Failed"] = PRECONDITION_FAILED;
    map["Payload Too Large"] = PAYLOAD_TOO_LARGE;
    map["URI Too Long"] = URI_TOO_LONG;
    map["Unsupported Media Type"] = UNSUPPORTED_MEDIA_TYPE;
    map["Range Not Satisfiable"] = RANGE_NOT_SATISFIABLE;
    map["Expectation Failed"] = EXPECTATION_FAILED;
    map["I'm a teapot"] = IM_A_TEAPOT;
    map["Misdirected Request"] = MISDIRECTED_REQUEST;
    map["Unprocessable Entity"] = UNPROCESSABLE_ENTITY;
    map["Locked"] = LOCKED;
    map["Failed Dependency"] = FAILED_DEPENDENCY;
    map["Too Early"] = TOO_EARLY;
    map["Upgrade Required"] = UPGRADE_REQUIRED;
    map["Precondition Required"] = PRECONDITION_REQUIRED;
    map["Too Many Requests"] = TOO_MANY_REQUESTS;
    map["Request Header Fields Too Large"] = REQUEST_HEADER_FIELDS_TOO_LARGE;
    map["Unavailable For Legal Reasons"] = UNAVAILABLE_FOR_LEGAL_REASONS;
    map["Internal Server Error"] = INTERNAL_SERVER_ERROR;
    map["Not Implemented"] = NOT_IMPLEMENTED;
    map["Bad Gateway"] = BAD_GATEWAY;
    map["Service Unavailable"] = SERVICE_UNAVAILABLE;
    map["Gateway Timeout"] = GATEWAY_TIMEOUT;
    map["HTTP Version Not Supported"] = HTTP_VERSION_NOT_SUPPORTED;
    map["Variant Also Negotiates"] = VARIANT_ALSO_NEGOTIATES;
    map["Insufficient Storage"] = INSUFFICIENT_STORAGE;
    map["Loop Detected"] = LOOP_DETECTED;
    map["Not Extended"] = NOT_EXTENDED;
    map["Network Authentication Required"] = NETWORK_AUTHENTICATION_REQUIRED;

    return ( map );
}
