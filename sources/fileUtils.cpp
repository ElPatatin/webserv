/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filesUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:16:02 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/21 16:50:32 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filesUtils.hpp"
#include "Exceptions.hpp"

std::fstream *  openFile( std::string config_path )
{
    std::fstream * config_file;
    try
    {
        if ( config_path.length() < 6 )
            throw FileNotConfigException( "Error: " + config_path + " is not a .conf file" );
        if ( ".conf" != config_path.substr( config_path.find_last_of( '.' ) ) )
            throw FileNotConfigException( "Error: " + config_path + " is not a .conf file" );

        config_file = new std::fstream;
        if ( config_file == NULL )
            throw MemoryAllocationException( "Error: memory allocation failed" );

        config_file->open( static_cast<const char *>( config_path.c_str() ), std::ios::in );
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