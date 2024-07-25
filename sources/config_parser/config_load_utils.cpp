/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_load_utils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:22:41 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/24 15:35:27 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parser.hpp"

std::string ConfigLoad::config_load_line( std::string * line, std::string error_message )
{
    size_t pos = line->find( " " );
    if ( pos == std::string::npos )
    {
        std::cerr << "Error: " << error_message << std::endl;
        LOG( ERROR ) << error_message;
        return ( "" );
    }

    *line = line->substr( pos + 1 );
    while ( !line->empty() && (*line)[line->size() - 1] == ';')
        line->erase(line->size() - 1);
    
    return ( *line );
}

bool ConfigLoad::config_load_port( std::string line, ConfigData & config_data )
{
    static volatile bool already_parsed = false;

    if ( already_parsed )
    {
        std::cerr << "Error: 'port' directive already parsed" << std::endl;
        LOG( ERROR ) << "'port' directive already parsed";
        return ( false );
    }

    config_load_line( &line, "'port' directive without port number" );
    if ( line.empty() )
        return ( false );

    unsigned short port = std::atoi( line.c_str() );

    std::stringstream ss;
    ss << port;
    if ( ss.str() != line  || ( port <= 0 || port > USHRT_MAX ) )
    {
        std::cerr << "Error: Invalid port number: " << port << std::endl;
        LOG( ERROR ) << "Invalid port number: " << port;
        return ( false );
    }

    config_data.setPort( static_cast< unsigned short >( port ));
    LOG( INFO ) << "Successfully parsed port: " << port;
    return ( true );
}

bool ConfigLoad::config_load_server_name( std::string line, ConfigData & config_data )
{
    ConfigLoad::config_load_line( &line, "'server_name' directive without server name" );
    if ( line.empty() )
        return ( false );

    std::vector< std::string > server_names = ft::split( line, " " );

    config_data.setHost( server_names[0] );
    config_data.setServerNames( server_names );
    LOG( INFO ) << "Successfully parsed host: " << server_names[0];
    LOG( INFO ) << "Successfully parsed server names: ";
    for ( size_t i = 0; i < server_names.size(); ++i )
        LOG(INFO) << "    " << server_names[i];
    return ( true );
}

bool ConfigLoad::config_load_error_page( std::string line, ConfigData & config_data )
{
    ConfigLoad::config_load_line( &line, "'error_page' directive without error code and/or path" );
    if ( line.empty() )
        return ( false );

    ErrorPages error_pages;

    std::vector< std::string > error_page = ft::split( line, " " );
    if ( error_page.size() != 2 )
    {
        std::cerr << "Error: 'error_page' directive with invalid number of arguments" << std::endl;
        LOG( ERROR ) << "'error_page' directive with invalid number of arguments";
        return ( false );
    }

    int error_code = std::atoi( error_page[0].c_str() );
    if ( error_code <= 0 )
    {
        std::cerr << "Error: Invalid error code: " << error_page[0] << std::endl;
        LOG( ERROR ) << "Invalid error code: " << error_page[0];
        return ( false );
    }

    error_pages[error_code] = error_page[1];
    config_data.setErrorPages( error_pages );
    LOG( INFO ) << "Successfully parsed error page: ";
    for ( std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it )
        LOG(INFO) << "    " << it->first << " -> " << it->second;
    return ( true );
}

bool ConfigLoad::config_load_client_max_body_size( std::string & line, ConfigData & config_data )
{
    ConfigLoad::config_load_line( &line, "'client_max_body_size' directive without size" );
    if ( line.empty() )
        return ( false );

    char c = std::toupper( line[line.size() - 1] );
    long size = std::atol( line.c_str() );

    if ( size <= 0 or size > 1024 )
    {
        std::cerr << "Error: Invalid size: " << line << std::endl;
        LOG( ERROR ) << "Invalid size: " << line;
        return ( false );
    }

    switch ( c )
    {
        case BYTE:
            size *= 1;          // 1024 ^ 0
            break;
        case KILO:
            size *= 1024;       // 1024 ^ 1
            break;
        case MEGA:
            size *= 1048576;    // 1024 ^ 2
            break;
        default:
            size = 1048576;     // 1024 ^ 2
    }

    config_data.setClientMaxBodySize( size );
    LOG( INFO ) << "Successfully parsed client max body size: " << config_data.getClientMaxBodySize();
    return ( true );
}

bool    ConfigLoad::config_load_root( std::string line, ConfigData & config_data )
{
    ConfigLoad::config_load_line( &line, "'root' directive without path" );
    if ( line.empty() )
        return ( false );

    config_data.setRoot( line );
    LOG( INFO ) << "Successfully parsed root: " << line;
    return ( true );
}

bool ConfigLoad::config_load_locations( const ConfigFile & config_file, std::string line, size_t line_num, ConfigData & config_data )
{
    std::vector< std::string > location = ft::split( line, " " );
    if ( location.size() != 3 )
    {
        std::cerr << "Error: 'location' directive with invalid number of arguments" << std::endl;
        LOG( ERROR ) << "'location' directive with invalid number of arguments";
        return ( false );
    }
    std::string endpoint = location[1];    

    std::vector< std::string > settings;
    for ( size_t i = line_num + 1; i < config_file.size(); ++i )
    {
        std::string line = config_file[i];
        if ( line.find( "}" ) != std::string::npos )
            break ;

        settings.push_back( line );
    }
    
    if ( settings.empty() )
    {
        std::cerr << "Error: 'location' directive without settings" << std::endl;
        LOG( ERROR ) << "'location' directive without settings";
        return ( false );
    }

    if ( settings.size() == 1 && settings[0].find( "return" ) != std::string::npos )
    {
        if ( !config_load_redirect( endpoint, settings[0], config_data ) )
            return ( false );
    }
    else
    {
        Locations   locations;
        Location    loc;
        std::vector< std::string > content;

        for ( size_t i = 0; i < settings.size(); ++i )
        {
            content = ft::split( settings[i], " " );

            // delete the ';' at the end of the line
            content[content.size() - 1].erase( content[content.size() - 1].size() - 1 );

            // Insert the content of the variables to a vector
            std::vector< std::string > values;
            for ( size_t j = 1; j < content.size(); ++j )
                values.push_back( content[j] );

            loc[content[0]] = values;            
        }
        locations.insert( std::make_pair( endpoint, loc ) );
        LOG( INFO ) << "Successfully parsed location: " << endpoint;
        config_data.setLocations( locations );
    }

    return ( true );
}

bool    ConfigLoad::config_load_redirect( std::string endpoint, std::string line, ConfigData & config_data )
{
    std::vector< std::string > redirect = ft::split( line, " " );

    if ( redirect.size() != 3 )
    {
        std::cerr << "Error: 'return' directive with invalid number of arguments" << std::endl;
        LOG( ERROR ) << "'return' directive with invalid number of arguments";
        return ( false );
    }

    // delete the ';' at the end of the line
    redirect[2].erase( redirect[2].size() - 1 );

    Redirects redirects;
    redirects[endpoint].first = ft::stoi( redirect[1] );
    redirects[endpoint].second = redirect[2];

    LOG( INFO ) << "Successfully parsed redirect: " << endpoint << " -> " << redirect[1] << " " << redirect[2];
    config_data.setRedirects( redirects );
    return ( true );
}

bool ConfigLoad::config_load_directory_listing( std::string line, ConfigData & config_data )
{
    // static volatile bool already_parsed = false;

    // if ( already_parsed )
    // {
    //     std::cerr << "Error: 'directory_listing' directive already parsed" << std::endl;
    //     LOG( ERROR ) << "'directory_listing' directive already parsed";
    //     return ( false );
    // }

    ConfigLoad::config_load_line( &line, "'directory_listing' directive without 'on' or 'off'" );
    if ( line.empty() )
        return ( false );

    if ( line == "on" )
        config_data.setIsDirectoryListing( true );
    else if ( line == "off" )
        config_data.setIsDirectoryListing( false );
    else
    {
        std::cerr << "Error: Invalid argument for 'directory_listing' directive: " << line << std::endl;
        LOG( ERROR ) << "Invalid argument for 'directory_listing' directive: " << line;
        return ( false );
    }

    // already_parsed = true;
    LOG( INFO ) << "Successfully parsed directory listing: " << ( config_data.getIsDirectoryListing() ? "on" : "off" );
    return ( true );

}
