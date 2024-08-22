/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:13:13 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/21 18:57:58 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_HPP
# define STRUCTS_HPP

# include "common.hpp"
# include <netinet/in.h>

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
    int         listen_sock;
    int         conn_sock;
    int         conn_fd;
    SockAddrIn  addr;
    size_t      addr_len;
    std::string response;
    size_t      bytes_sent;
}   Data;

#endif