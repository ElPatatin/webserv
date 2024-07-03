/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/03 16:27:37 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LoadConfig.hpp"
#include "ConfigData.hpp"
#include "webserver.hpp"

/**
 * @brief If the arguments are adequate, checks if the config file is properly structured.
 * Then, if posible set ups as many servers as the config file specifies.
 * Finnaly, starts the webserver.
*/
int main(int ac, char **av)
{
    try
    {
        LOG( INFO ) << "--[ Program started ]--";

        if (ac < 1 || ac > 2)
            throw BadArrgumentsException( "Usage: ./webserv [config_file]" );

        if ( !LoadConfig::checkConfig() )
            return ( 1 );

        ConfigData config;
        LoadConfig::loadConfig( ac, av, &config );

        ft::welcome();
        webserver( config );
    }
    catch( BadArrgumentsException & e ) { std::cerr << e.what() << std::endl; return ( 1 ); }
    catch( ConfigFileException & e ) { std::cerr << e.what() << std::endl; return ( 2 ); }
    catch( ... ) { std::cerr << "Error: unknown exception" << std::endl; return ( 255 ); }

    return ( 0 );
}