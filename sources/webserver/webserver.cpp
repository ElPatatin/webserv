/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:39:28 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/09 19:55:31 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"
#include "http.hpp"

namespace g_signal { volatile sig_atomic_t g_signal_status = true; }

static bool startServer( ConfigData & config, Addrs & addrs, Data & data );
static bool runServer( Data & data, EpollData & epoll, ConfigData & config );
static bool stopServer( Data & data, EpollData & epoll );

//  Sets up the signals for the servers and manages the webserver.
void    webserver( ConfigData &config )
{
    Addrs       addrs;
    Data        data;
    EpollData   epoll;

    std::signal( SIGINT, signalHandler );
    std::signal( SIGQUIT, signalHandler );

    if ( !startServer( config, addrs, data ) )
        return ;

    if ( !runServer( data, epoll, config ) )
        return ;

    if ( !stopServer( data, epoll ) )
        return ;

    return ;
}

// Starts the server and binds the socket.
static bool startServer( ConfigData & config, Addrs & addrs, Data & data )
{
    LOG( INFO ) << "Starting server";
    std::cout << "Starting server" << std::endl;

    try
    {
        // AF_INET: IPv4, SOCK_STREAM: TCP
        addrs = Sockets::resolveHostToIp( AF_INET, SOCK_STREAM, config.getHost() );

        data.listen_sock = Sockets::createSocket( addrs.rp );
        Sockets::bindSocket( &data, config );

        if ( !addrs.rp )
            throw ResolveHostException( "Error: failed to bind" );

        freeaddrinfo( addrs.result );

        Sockets::listenConnection( data, BACKLOG );
    }
    catch( ResolveHostException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( SocketException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( ... ) { return ( std::cerr << "Error: unknown exception" << std::endl, false ); }

    LOG( INFO ) << "Server started";
    std::cout << "Server started" << std::endl;
    return ( true );
}

// Runs the listen sockets and accepts the incoming connections.
static bool runServer( Data & data, EpollData & epoll, ConfigData & config )
{
    LOG( INFO ) << "Running server";
    std::cout << "Running server" << std::endl;

    try
    {
        Epoll::createEpoll( epoll );
        Epoll::addEpoll( epoll, data.listen_sock, EPOLLIN );

        while ( g_signal::g_signal_status )
        {
            Epoll::waitEpoll( epoll );
            if ( epoll.nfds == -1 )
            {
                std::cout << "Timeout occurred, no events happened" << std::endl;
                continue ;
            }

            for ( int i = 0; i < epoll.nfds; ++i )
            {
                if ( epoll.events[ i ].data.fd == data.listen_sock )
                {
                    Sockets::acceptConnection( data );
                    Sockets::setSocketBlockingMode( data.conn_sock, false );
                    Epoll::addEpoll( epoll, data.conn_sock, EPOLLIN | EPOLLET );
                }
                else
                {
                    data.conn_fd = epoll.events[ i ].data.fd;
                    Sockets::receiveConnection( data, config );
                    Sockets::sendConnection( data );
                    Sockets::closeConnection( data.conn_fd, __FUNCTION__, __LINE__ );
                }
            }
        }
    }
    catch( SocketException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( EpollException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( ... ) { return ( std::cerr << "Error: unknown exception" << std::endl, false ); }

    LOG( INFO ) << "Server stopped";
    std::cout << "Server stopped" << std::endl;
    return ( true );
}

// Stops the server and closes the connection.
static bool stopServer( Data & data, EpollData & epoll )
{
    LOG( INFO ) << "Stopping server";
    
    UNUSED( data );
    UNUSED( epoll );
    // Epoll::removeEpoll( data, epoll );
    // Sockets::closeConnection( data.conn_fd, __FUNCTION__, __LINE__ );
    return ( true );
}