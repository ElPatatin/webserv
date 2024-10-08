/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:39:28 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/22 00:32:49 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

//  Sets up the signals for the servers and manages the webserver.
void    webserver( Cluster & cluster )
{
    Servers servers;

    std::signal( SIGINT, signalHandler );
    std::signal( SIGQUIT, signalHandler );

    for ( size_t i = 0; i < cluster.n_servers; ++i )
    {
        if ( !WebServer::start_server( cluster.config_data[ i ], servers ) )
            return ;
    }

    if ( !WebServer::run_server( servers ) )
        return ;

    if ( !WebServer::stop_server( servers ) )
        return ;

    return ;
}
