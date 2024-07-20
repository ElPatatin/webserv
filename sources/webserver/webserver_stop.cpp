/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver_stop.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:36:44 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/20 11:39:28 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

// Stops the server and closes the connection.
bool WebServer::stop_server( Servers & servers )
{
    LOG( INFO ) << "Stopping server";
    
    for ( Servers::iterator it = servers.begin(); it != servers.end(); ++it )
    {
        ConectionSockets::closeConnection( it->second.data.listen_sock, __FUNCTION__, __LINE__ );
    }

    return ( true );
}
