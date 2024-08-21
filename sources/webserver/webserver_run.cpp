/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver_run.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 11:37:02 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/21 21:40:56 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

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
            uint32_t events = epoll.events[ i ].events;

            Servers::iterator it = servers.find( event_fd );
            if ( it != servers.end() )
                WebServer::handle_new_connection( event_fd, servers, epoll, connection_to_server_map );
            else
                WebServer::handle_existing_connection( event_fd, events, epoll, connection_to_server_map );
        }
    }

    return ( true );
}

void    WebServer::handle_new_connection
(
    int event_fd,
    Servers & servers,
    EpollData & epoll,
    std::map < int, ServerData * > & connection_to_server_map
)
{
    // New connection on listening socket
    Servers::iterator it = servers.find( event_fd );
    if ( it == servers.end() )
    {
        LOG( ERROR ) << "Unknown listening socket event";
        return ;
    }

    LOG( INFO ) << "New connection on port " << it->second.config->getPort();
    Sockets::acceptConnection( it->second.data );
    Sockets::setSocketBlockingMode( it->second.data.conn_sock, false );
    Epoll::add_epoll( epoll, it->second.data.conn_sock, EPOLLIN | EPOLLET | EPOLLHUP | EPOLLERR );

    // Track the new connection with the correct servers
    connection_to_server_map[ it->second.data.conn_sock ] = &it->second;

    return ;
}

void    WebServer::handle_existing_connection
(
    int event_fd,
    uint32_t events,
    EpollData & epoll,
    std::map < int, ServerData * > & connection_to_server_map
)
{
    std::map< int, ServerData * >::iterator connIt = connection_to_server_map.find( event_fd );
    if ( connIt == connection_to_server_map.end() )
    {
        LOG( ERROR ) << "Unknown connection event";
        return ;
    }

    ServerData *serverData = connIt->second;
    serverData->data.conn_fd = event_fd;
    LOG( INFO ) << "Connection event on port " << serverData->config->getPort();
    
    if ( events & ( EPOLLERR | EPOLLHUP ) )
    {
        LOG( INFO ) << "An error ocrrured on connection or client closed connection";
        Sockets::closeConnection( serverData->data.conn_fd, __FUNCTION__, __LINE__ );
        connection_to_server_map.erase( connIt );
        WebServer::resetConnection( serverData );
        return ;
    }

    if ( serverData->state == READING_REQUEST && events & EPOLLIN )
        WebServer::handle_reading_request( epoll, serverData, connection_to_server_map, connIt );

    if ( serverData->state == PROCESSING_REQUEST )
        WebServer::process_request( event_fd, epoll, serverData );

    if ( serverData->state == SENDING_RESPONSE && events & EPOLLOUT )
        WebServer::handle_sending_response( epoll, serverData, connection_to_server_map, connIt );

    if (serverData->state == CLOSING)
        WebServer::close_connection( event_fd, connection_to_server_map, connIt, serverData );

    return ;
}
