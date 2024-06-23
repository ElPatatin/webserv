/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 16:17:23 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LoadConfig.hpp"
#include "ConfigData.hpp"
#include "Sock.hpp"

int main(int ac, char **av)
{
    ConfigData  config;

    LOG(INFO) << "Starting server";
    
    if (ac < 1 || ac > 2)
        throw BadArrgumentsException( "Usage: ./webserv [config_file]" );

    LoadConfig::loadConfig( ac, av, &config );
    if ( !LoadConfig::checkConfig( config ) )
        return ( 1 );
    
    return 0;

    std::signal( SIGINT, Sock::handleSignal );
    std::signal( SIGTERM, Sock::handleSignal );
    std::signal( SIGQUIT, Sock::handleSignal );

    Sock sock( AF_INET, SOCK_STREAM, 0, 8080, "localhost" );
    
    return ( 0 );
}