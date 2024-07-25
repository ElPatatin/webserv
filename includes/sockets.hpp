/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:06:48 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/25 12:53:03 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# include "common.hpp"
# include "ConfigData.hpp"
# include "Exceptions.hpp"
# include "Log.hpp"
# include "structs.hpp"

# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# define BACKLOG 10
# define CHUNK_SIZE 4096

namespace Sockets
{
    Addrs   resolveHostToIp( int domain, int type, std::string host );
    int     createSocket( AddrInfo *rp );
    void    bindSocket( Data *data, ConfigData & config );
    void    listenConnection( Data & data, int backlog );
    void    acceptConnection( Data & data );
    ssize_t receiveConnection( const Data & data, char * buffer, size_t size );
    ssize_t sendConnection( const Data & data, const char * buffer, size_t size );
    void    closeConnection( int fd, std::string function, int line );
    void    setSocketBlockingMode( int sockfd, bool blocking );
}

namespace CommunicationSockets
{
    void    receiveConnection( Data & data, ConfigData & config );
    bool    headersReceived( const std::string & request, int & content_length );
    void    sendConnection( Data & data );
    int     waitTime( const Data & data, const bool & is_read );
}

#endif