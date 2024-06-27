/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:31:58 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/27 20:04:27 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "common_dependencies.hpp"
# include "ConfigData.hpp"
# include "Exceptions.hpp"
# include "Log.hpp"
# include "utils.hpp"

# include <csignal>
# include <netdb.h>

# define UNUSED(x) (void)(x)

typedef struct addrinfo     AddrInfo;
typedef struct sockaddr_in  SockAddrIn;
typedef struct sockaddr     SockAddr;

typedef struct s_addrs
{
    AddrInfo    *result;
    AddrInfo    *rp;
}   Addrs;

typedef struct s_data
{
    int         conn_fd;
    SockAddrIn  addr;
    size_t      addr_len;
}   Data;

namespace g_signal { extern volatile sig_atomic_t g_signal_status; }

// Signals
void    signalHandler( int signum );

// Webserver
void    webserver( ConfigData config );

namespace Sockets
{
    Addrs   resolveHostToIp( int domain, int type, std::string host );
    int     createSocket( AddrInfo *rp );
    void    bindSocket( Data *data, ConfigData config );
}

#endif