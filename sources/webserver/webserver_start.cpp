/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver_start.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:37:04 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/22 00:28:13 by cpeset-c         ###   ########.fr       */
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
        server_data.addrs = Sockets::resolveHostToIp( AF_INET, SOCK_STREAM, config.getHost() );

        server_data.data.listen_sock = Sockets::createSocket( server_data.addrs.rp );
        Sockets::bindSocket( &server_data.data, config );

        if ( !server_data.addrs.rp )
            throw ResolveHostException( "Error: failed to bind" );

        freeaddrinfo( server_data.addrs.result );

        Sockets::listenConnection( server_data.data, BACKLOG );

        servers[ server_data.data.listen_sock ] = server_data;
    }
    catch( ResolveHostException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( SocketException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( ... ) { return ( std::cerr << "Error: unknown exception" << std::endl, false ); }

    LOG( INFO ) << "Server started";
    std::cout << "Server started" << std::endl;
    return ( true );
}
