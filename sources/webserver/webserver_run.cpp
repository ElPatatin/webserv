/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver_run.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:37:02 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/25 17:48:13 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"
#include "http.hpp"

namespace g_signal { volatile sig_atomic_t g_signal_status = true; }

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

        Sockets::acceptConnection( it->second.data );
        Sockets::setSocketBlockingMode( it->second.data.conn_sock, false );
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
        std::string request = CommunicationSockets::receiveConnection( serverData->data );
        if ( request.empty() )
        {
            LOG( INFO ) << "Connection closed by client";
            Sockets::closeConnection( serverData->data.conn_fd, __FUNCTION__, __LINE__ );
            connection_to_server_map.erase( connIt );
            return ;
        }

        HttpData http = HttpRequests::parseRequest( request );
        Http::httpRequest( http, serverData->data, *serverData->config );

        CommunicationSockets::sendConnection( serverData->data );
        Sockets::closeConnection( serverData->data.conn_fd, __FUNCTION__, __LINE__ );
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
