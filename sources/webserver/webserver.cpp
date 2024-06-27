/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:39:28 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/27 20:18:19 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserver.hpp"

namespace g_signal { volatile sig_atomic_t g_signal_status = true; }

static bool    binding( ConfigData config );

void    webserver( ConfigData config )
{
    std::signal( SIGINT, signalHandler );
    std::signal( SIGQUIT, signalHandler );

    if ( !binding( config ) )
        return ;

    while ( g_signal::g_signal_status )
    {
        ; // Do something   
    }

    return ;
}

static bool    binding( ConfigData config )
{
    Addrs   addrs;
    Data    data;

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

    return ( true );
}

