/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/22 01:40:42 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigData.hpp"
#include "webserver.hpp"
#include "config_parser.hpp"

/**
 * @brief If the arguments are adequate, checks if the config file is properly structured.
 * Then, if posible set ups as many servers as the config file specifies.
 * Finnaly, starts the webserver.
*/
int main(int ac, char **av)
{
    try
    {
        // Delete previous log file if it exists
        if ( std::ifstream( "webserver.log" ) )
            std::remove( "webserver.log" );
        LOG( INFO ) << "--[ Program started ]--";

        if (ac < 1 || ac > 2)
            throw BadArrgumentsException( "Usage: ./webserv [config_file]" );

        Cluster cluster;
        config_parser( ac, av, cluster );

        ft::welcome();
        webserver( cluster );
        return ( 0 );
    }
    catch( BadArrgumentsException & e ) { std::cerr << e.what() << std::endl; return ( 1 ); }
    catch( ConfigFileException & e ) { std::cerr << e.what() << std::endl; return ( 2 ); }
    catch( std::exception & e ) { std::cerr << e.what() << std::endl; return ( 3 ); }

    return ( 0 );
}