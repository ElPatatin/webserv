/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:16:02 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 17:02:25 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Exceptions.hpp"

std::fstream *  ft::openFile( const std::string & file_path, std::ios::openmode _mode )
{
    std::fstream * config_file = NULL;
    try
    {
        config_file = new std::fstream;
        if ( config_file == NULL )
            throw MemoryAllocationException( "Error: memory allocation failed" );

        config_file->open( file_path.c_str(), _mode );
        if ( config_file->fail() )
            throw FileNotOpenException( "Error: " + file_path + " could not be opened" );
        if ( !config_file->is_open() )
            throw FileNotOpenException( "Error: " + file_path + " not found" );
            
        return ( config_file );
    }
    catch ( FileNotConfigException & e )
    {
        std::cerr << e.what() << std::endl;
        return ( deleteOpenFile( config_file ) );
    }
    catch ( MemoryAllocationException & e )
    {
        std::cerr << e.what() << std::endl;
        return ( deleteOpenFile( config_file ) );
    }
    catch ( FileNotOpenException & e )
    {
        std::cerr << e.what() << std::endl;
        return ( deleteOpenFile( config_file ) );
    }
    catch ( ... )
    {
        std::cerr << "Error: unknown exception" << std::endl;
        return ( deleteOpenFile( config_file ) );
    }
}

void    ft::closeFile( std::fstream * _file )
{
    if ( _file )
    {
        try
        {
            if ( _file->is_open() )
            {
                _file->clear();
                _file->close();
            }
            if ( _file->fail() )
                throw FileNotCloseException( "Error: file could not be closed" );
            deleteOpenFile( _file );
        }
        catch( FileNotCloseException & e )
        {
            std::cerr << e.what() << std::endl;
            deleteOpenFile( _file );
        }
        catch( ... )
        {
            std::cerr << "Error: unknown exception" << std::endl;
            deleteOpenFile( _file );
        }
        
    }
    return ;
}

std::fstream *  ft::deleteOpenFile( std::fstream * _file )
{
    if ( _file )
    {
        delete _file;
        _file = NULL;
    }
    return ( NULL );
}

/**
 * @brief Remove leading and trailing spaces from a string
 * 
 * @param str The string to trim
 * @return void
 * 
 * @details
 * First, find the first non-space character from the beginning of the string.
 * If all characters are spaces, the string is empty or spaces only.
 * Then, find the first non-space character from the end of the string.
 * Erase everything from the end not found.
 * Erase everything from the beginning not found.
 * 
 * @see https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
void ft::trim( std::string& str )
{
    size_t startpos = str.find_first_not_of( " \t\r\n" );

    if ( std::string::npos == startpos )
    {
        str = "";
        return ;
    }

    size_t endpos = str.find_last_not_of( " \t\r\n" );

    str.erase( endpos + 1 );
    str.erase( 0, startpos );

    return ;
}

std::string& ft::rtrim( std::string & str )
{
    size_t endpos = str.find_last_not_of( " \t" );
    if ( std::string::npos != endpos )
        str = str.substr( 0, endpos + 1 );

    return ( str );
}

std::vector< std::string > ft::split( const std::string & str, std::string delimiter )
{
    std::vector< std::string > tokens;
    size_t start = 0;
    size_t end = str.find( delimiter );

    while ( end != std::string::npos )
    {
        tokens.push_back( str.substr( start, end - start ) );
        start = end + delimiter.length();
        end = str.find( delimiter, start );
    }

    if ( start < str.length() )
        tokens.push_back( str.substr( start, end - start ) );

    return ( tokens );
}

std::string ft::prettyPrint( std::string function, int line, std::string message )
{
    std::stringstream pretty;
    pretty << "In function " << function << " at line " << line << ": " << message;
    return ( pretty.str() );
}

void    ft::welcome( void )
{
    std::fstream * header = ft::openFile( "./welcome.txt", std::ios::in );
    if ( header )
    {
        std::string line;
        while ( std::getline( *header, line ) )
            std::cout << line << std::endl;
        ft::closeFile( header );
    }
}

int    ft::stoi( std::string str )
{
    std::stringstream ss( str );
    int i;
    ss >> i;
    return ( i );
}

std::string ft::itos( const int & value )
{
    std::ostringstream oss;
    oss << value;
    return ( oss.str() );
}

std::string ft::join( const std::vector< std::string > & vec, const std::string & delimiter )
{
    std::string result = "";

    for ( size_t i = 0; i < vec.size(); ++i )
    {
        if ( vec.size() - 1 == i )
            result += vec[ i ];
        else
            result += vec[ i ] + delimiter;
    }

    return ( result );
}