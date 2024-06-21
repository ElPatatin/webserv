/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:22:53 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/21 23:34:08 by cpeset-c         ###   ########.fr       */
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

class MemoryAllocationException : public std::runtime_error
{
    public:
        MemoryAllocationException( std::string const & message )
            : std::runtime_error( message ) { return ; }
        ~MemoryAllocationException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class ConfigFileException : public std::runtime_error
{
    public:
        ConfigFileException( std::string const & message )
            : std::runtime_error( message ) { return ; }
        ~ConfigFileException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

class FileParseException : public std::runtime_error
{
    public:
        FileParseException( std::string const & message )
            : std::runtime_error( message ) { return ; }
        ~FileParseException() throw() { return ; }
        const char* what() const throw() { return msg.c_str(); }
    private:
        std::string msg;
};

#endif