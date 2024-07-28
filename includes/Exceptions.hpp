/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:22:53 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/28 15:27:01 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include <stdexcept>
#include <exception>
#include <string>


// EXCEPTIONS
// ==========

class BadArrgumentsException : public std::exception
{
    public:
        BadArrgumentsException( const std::string& message )
            : msg( message ) { return ; }
        ~BadArrgumentsException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class FileNotConfigException : public std::exception
{
    public:
        FileNotConfigException( const std::string& message )
            : msg( message ) { return ; }
        ~FileNotConfigException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class FileNotOpenException : public std::exception
{
    public:
        FileNotOpenException( const std::string& message )
            : msg( message ) { return ; }
        ~FileNotOpenException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class FileNotCloseException : public std::exception
{
    public:
        FileNotCloseException( const std::string& message )
            : msg( message ) { return ; }
        ~FileNotCloseException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class MemoryAllocationException : public std::exception
{
    public:
        MemoryAllocationException( std::string const & message )
            : msg( message ) { return ; }
        ~MemoryAllocationException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class ConfigFileException : public std::exception
{
    public:
        ConfigFileException( std::string const & message )
            : msg( message ) { return ; }
        ~ConfigFileException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class FileParseException : public std::exception
{
    public:
        FileParseException( std::string const & message )
            : msg( message ) { return ; }
        ~FileParseException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class ResolveHostException : public std::exception
{
    public:
        ResolveHostException( std::string const & message )
            : msg( message ) { return ; }
        ~ResolveHostException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class SocketException : public std::exception
{
    public:
        SocketException( std::string const & message )
            : msg( message ) { return ; }
        ~SocketException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class EpollException : public std::exception
{
    public:
        EpollException( std::string const & message )
            : msg( message ) { return ; }
        ~EpollException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class MethodNotAllowedException : public std::exception
{
    public:
        MethodNotAllowedException( std::string const & message )
            : msg( message ) { return ; }
        ~MethodNotAllowedException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class HttpHeaderException : public std::exception
{
    public:
        HttpHeaderException( std::string const & message )
            : msg( message ) { return ; }
        ~HttpHeaderException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class DirectoryListingException : public std::exception
{
    public:
        DirectoryListingException( std::string const & message )
            : msg( message ) { return ; }
        ~DirectoryListingException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class ForkException : public std::exception
{
    public:
        ForkException( std::string const & message )
            : msg( message ) { return ; }
        ~ForkException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class ExecveException : public std::exception
{
    public:
        ExecveException( std::string const & message )
            : msg( message ) { return ; }
        ~ExecveException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class HttpVersionException : public std::exception
{
    public:
        HttpVersionException( std::string const & message )
            : msg( message ) { return ; }
        ~HttpVersionException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class HttpMethodsException : public std::exception
{
    public:
        HttpMethodsException( std::string const & message )
            : msg( message ) { return ; }
        ~HttpMethodsException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class HttpUrlException : public std::exception
{
    public:
        HttpUrlException( std::string const & message )
            : msg( message ) { return ; }
        ~HttpUrlException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class HttpQueryException : public std::exception
{
    public:
        HttpQueryException( std::string const & message )
            : msg( message ) { return ; }
        ~HttpQueryException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class HttpBodyException : public std::exception
{
    public:
        HttpBodyException( std::string const & message )
            : msg( message ) { return ; }
        ~HttpBodyException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class HttpHeadersException : public std::exception
{
    public:
        HttpHeadersException( std::string const & message )
            : msg( message ) { return ; }
        ~HttpHeadersException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class HttpContentLengthException : public std::exception
{
    public:
        HttpContentLengthException( std::string const & message )
            : msg( message ) { return ; }
        ~HttpContentLengthException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class HttpException : public std::exception
{
    public:
        HttpException( std::string const & message )
            : msg( message ) { return ; }
        ~HttpException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

#endif