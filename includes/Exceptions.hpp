/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:22:53 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 17:06:43 by cpeset-c         ###   ########.fr       */
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

#endif