/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 18:02:05 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LoadConfig.hpp"
#include "ConfigData.hpp"
#include "Sock.hpp"

int main(int ac, char **av)
{
    ConfigData  config;
    LOG(INFO) << "Starting server";

    try
    {
        if (ac < 1 || ac > 2)
            throw BadArrgumentsException( "Usage: ./webserv [config_file]" );

        LoadConfig::loadConfig( ac, av, &config );
        if ( !LoadConfig::checkConfig( config ) )
            return ( 1 );
            
        std::signal( SIGINT, Sock::handleSignal );
        std::signal( SIGQUIT, Sock::handleSignal );

        int nserver = config.getNestedServers().size() + 1;
        for ( int i = 0; i < nserver; i++ )
            Sock sock( AF_INET, SOCK_STREAM, 0, config.getPort(), config.getHost() );

    }
    catch( BadArrgumentsException & e )
    {
        std::cerr << e.what() << std::endl;
        return ( 1 );
    }
    catch( ConfigFileException & e )
    {
        std::cerr << e.what() << std::endl;
        return ( 2 );
    }
    catch( ... )
    {
        std::cerr << "Error: unknown exception" << std::endl;
        return ( 255 );
    }

    return ( 0 );
}