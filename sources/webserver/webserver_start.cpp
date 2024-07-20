/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver_start.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:37:04 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/20 11:37:44 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

// Starts the server and binds the socket.
bool WebServer::start_server( ConfigData & config, Servers & servers )
{
    LOG( INFO ) << "Starting server";
    std::cout << "Starting server" << std::endl;
    ServerData server_data;
    server_data.config = &config;

    try
    {
        // AF_INET: IPv4, SOCK_STREAM: TCP
        server_data.addrs = ConectionSockets::resolveHostToIp( AF_INET, SOCK_STREAM, config.getHost() );

        server_data.data.listen_sock = ConectionSockets::createSocket( server_data.addrs.rp );
        ConectionSockets::bindSocket( &server_data.data, config );

        if ( !server_data.addrs.rp )
            throw ResolveHostException( "Error: failed to bind" );

        freeaddrinfo( server_data.addrs.result );

        ConectionSockets::listenConnection( server_data.data, BACKLOG );

        servers[ server_data.data.listen_sock ] = server_data;
    }
    catch( ResolveHostException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( SocketException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( ... ) { return ( std::cerr << "Error: unknown exception" << std::endl, false ); }

    LOG( INFO ) << "Server started";
    std::cout << "Server started" << std::endl;
    return ( true );
}
