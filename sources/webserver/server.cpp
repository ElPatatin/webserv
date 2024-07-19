/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:46:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/19 20:03:11 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

namespace g_signal { volatile sig_atomic_t g_signal_status = true; }

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

// Runs the listen sockets and accepts the incoming connections.
bool    WebServer::run_server( Servers & servers )
{
    LOG( INFO ) << "Running servers";

    EpollData                       epoll;
    std::map < int, ServerData * >  connection_to_server_map;

    try
    {
        Epoll::create_epoll( epoll );
        WebServer::add_listening_sockets_to_epoll( servers, epoll );
        if ( !run_server_loop( servers, epoll, connection_to_server_map ) )
            return ( false );
    }
    catch ( SocketException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch ( EpollException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch ( ... ) { return ( std::cerr << "Error: unknown exception" << std::endl, false ); }

    LOG( INFO ) << "Servers stopped";
    std::cout << "Servers stopped" << std::endl;
    return ( true );
}

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

bool    WebServer::run_server_loop( Servers & servers, EpollData & epoll, std::map < int, ServerData * > & connection_to_server_map )
{
    while ( g_signal::g_signal_status )
    {
        LOG( INFO ) << "Waiting for events";

        Epoll::wait_epoll( epoll );
        if ( epoll.nfds == -1 )
        {
            std::cout << "Timeout occurred, no events happened" << std::endl;
            continue;
        }

        for ( int i = 0; i < epoll.nfds; ++i )
        {
            int event_fd = epoll.events[ i ].data.fd;

            Servers::iterator it = servers.find( event_fd );
            if ( it != servers.end() )
                WebServer::handle_new_connection( event_fd, servers, epoll, connection_to_server_map );
            else
                WebServer::handle_existing_connection( event_fd, connection_to_server_map );
        }
    }

    return ( true );
}

void    WebServer::handle_new_connection( int event_fd, Servers & servers, EpollData & epoll, std::map < int, ServerData * > & connection_to_server_map )
{
    // New connection on listening socket
    Servers::iterator it = servers.find( event_fd );

    if ( it != servers.end() )
    {
        LOG( INFO ) << "New connection on port " << it->second.config->getPort();

        ConectionSockets::acceptConnection( it->second.data );
        ConectionSockets::setSocketBlockingMode( it->second.data.conn_sock, false );
        Epoll::add_epoll( epoll, it->second.data.conn_sock, EPOLLIN | EPOLLET | EPOLLHUP | EPOLLERR );

        // Track the new connection with the correct server
        connection_to_server_map[ it->second.data.conn_sock ] = &it->second;
    } else
        LOG( ERROR ) << "Unknown listening socket event";

    return ;
}

void    WebServer::handle_existing_connection( int event_fd, std::map < int, ServerData * > & connection_to_server_map )
{
    std::map< int, ServerData * >::iterator connIt = connection_to_server_map.find( event_fd );

    // Connection event on a previously accepted socket
    if ( connIt != connection_to_server_map.end() )
    {
        ServerData *serverData = connIt->second;
        LOG( INFO ) << "Connection event on port " << serverData->config->getPort();

        serverData->data.conn_fd = event_fd;
        CommunicationSockets::receiveConnection( serverData->data, *serverData->config );
        CommunicationSockets::sendConnection( serverData->data );
        ConectionSockets::closeConnection( serverData->data.conn_fd, __FUNCTION__, __LINE__ );
        connection_to_server_map.erase( connIt );
    } else {
        LOG( ERROR ) << "Unknown connection event";
    }
}

void    WebServer::add_listening_sockets_to_epoll( Servers & servers, EpollData & epoll )
{
    // Add all listening sockets to epoll
    for ( Servers::iterator it = servers.begin(); it != servers.end(); ++it )
    {
        LOG( INFO ) << "Server listening on port " << it->second.config->getPort();
        Epoll::add_epoll( epoll, it->second.data.listen_sock, EPOLLIN );
    }
}
