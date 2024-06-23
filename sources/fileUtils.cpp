/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:16:02 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 17:02:25 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fileUtils.hpp"
#include "Exceptions.hpp"

std::fstream *  openFile( std::string config_path )
{
    std::fstream * config_file = NULL;
    try
    {
        if ( config_path.length() < 6 )
            throw FileNotConfigException( "Error: " + config_path + " is not a .conf file" );
        if ( ".conf" != config_path.substr( config_path.find_last_of( '.' ) ) )
            throw FileNotConfigException( "Error: " + config_path + " is not a .conf file" );

        config_file = new std::fstream;
        if ( config_file == NULL )
            throw MemoryAllocationException( "Error: memory allocation failed" );

        config_file->open( static_cast< const char * >( config_path.c_str() ), std::ios::in | std::ios::out );
        if ( config_file->fail() )
            throw FileNotOpenException( "Error: " + config_path + " could not be opened" );
        if ( !config_file->is_open() )
            throw FileNotOpenException( "Error: " + config_path + " not found" );
            
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

void    closeFile( std::fstream * config_file )
{
    if ( config_file )
    {
        try
        {
            if ( config_file->is_open() )
            {
                config_file->clear();
                config_file->close();
            }
            if ( config_file->fail() )
                throw FileNotCloseException( "Error: file could not be closed" );
            deleteOpenFile( config_file );
        }
        catch( FileNotCloseException & e )
        {
            std::cerr << e.what() << std::endl;
            deleteOpenFile( config_file );
        }
        catch( ... )
        {
            std::cerr << "Error: unknown exception" << std::endl;
            deleteOpenFile( config_file );
        }
        
    }
    return ;
}

std::fstream *  deleteOpenFile( std::fstream * config_file )
{
    if ( config_file )
    {
        delete config_file;
        config_file = NULL;
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
void trim( std::string& str )
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

std::string& rtrim( std::string & str )
{
    size_t endpos = str.find_last_not_of( " \t" );
    if ( std::string::npos != endpos )
        str = str.substr( 0, endpos + 1 );

    return ( str );
}

std::vector< std::string > split( const std::string & str, char delimiter )
{
    std::vector< std::string > tokens;
    std::istringstream iss( str );
    std::string token;

    while ( std::getline( iss, token, delimiter ) )
        tokens.push_back( token );

    return ( tokens );
}