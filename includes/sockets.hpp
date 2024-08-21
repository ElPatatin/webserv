/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:06:48 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/21 19:41:31 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# include "common.hpp"
# include "ConfigData.hpp"
# include "Exceptions.hpp"
# include "Log.hpp"
# include "structs.hpp"
# include "epoll.hpp"

# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# ifndef BACKLOG
#  define BACKLOG 10
# endif

# ifndef CHUNK_SIZE
#  define CHUNK_SIZE 4096
# endif

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

#endif