/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:39:28 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/29 22:25:09 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserver.hpp"

namespace g_signal { volatile sig_atomic_t g_signal_status = true; }

static bool startServer( ConfigData config, Addrs & addrs, Data & data );
static bool runServer( ConfigData config, Addrs & addrs, Data & data, EpollData & epoll );
static bool stopServer( ConfigData config );

void    webserver( ConfigData config )
{
    Addrs       addrs;
    Data        data;
    EpollData   epoll;

    std::signal( SIGINT, signalHandler );
    std::signal( SIGQUIT, signalHandler );

    if ( !startServer( config, addrs, data ) )
        return ;

    if ( !runServer( config, addrs, data, epoll ) )
        return ;

    if ( !stopServer( config ) )
        return ;

    return ;
}

static bool startServer( ConfigData config, Addrs & addrs, Data & data )
{
    LOG( INFO ) << "Starting server";
    std::cout << "Starting server" << std::endl;

    try
    {
        addrs = Sockets::resolveHostToIp( AF_INET, SOCK_STREAM, config.getHost() );

        data.conn_fd = Sockets::createSocket( addrs.rp );
        Sockets::bindSocket( &data, config );

        if ( !addrs.rp )
            throw ResolveHostException( "Error: failed to bind" );

        freeaddrinfo( addrs.result );
    }
    catch( ResolveHostException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( SocketException & e ) { return ( std::cerr << e.what() << std::endl, false ); }
    catch( ... ) { return ( std::cerr << "Error: unknown exception" << std::endl, false ); }

    LOG( INFO ) << "Server started";
    std::cout << "Server started" << std::endl;
    return ( true );
}

static bool runServer( ConfigData config, Addrs & addrs, Data & data, EpollData & epoll )
{
    UNUSED( addrs );
    UNUSED( config );
    LOG( INFO ) << "Running server";
    std::cout << "Running server" << std::endl;

    try
    {
        Sockets::listenConnection( data, BACKLOG );
        Epoll::createEpoll( data, epoll );
        Epoll::addEpoll( data, epoll );

        while ( g_signal::g_signal_status )
        {
            Epoll::waitEpoll( data, epoll );
            if ( epoll.nfds == -1 )
            {
                std::cout << "Timeout occurred, no events happened" << std::endl;
                return ( true );
            }

            for ( int i = 0; i < epoll.nfds; i++ )
            {
                if ( epoll.events[ i ].data.fd == data.conn_fd )
                {
                    Sockets::acceptConnection( &data );
                    Epoll::addEpoll( data, epoll );
                }
                else
                {
                    Sockets::receiveConnection( &data );
                    Epoll::updateEpoll( data, epoll );
                }

                Sockets::closeConnection( &data, __FUNCTION__, __LINE__ );
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

static bool stopServer( ConfigData config )
{
    UNUSED( config );
    return ( true );
}