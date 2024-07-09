/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epollManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:40:42 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/09 15:58:29 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

void    Epoll::createEpoll( EpollData & epoll )
{
    epoll.epoll_fd = epoll_create( 1 );
    if ( epoll.epoll_fd == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_create: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_create1: " + std::string( std::strerror( errno ) ) );
    }
}

void    Epoll::addEpoll( EpollData & epoll, int sock, int events )
{
    epoll.event.data.fd = sock;
    epoll.event.events = events;

    if ( epoll_ctl( epoll.epoll_fd, EPOLL_CTL_ADD, sock, &epoll.event ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_ctl: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_ctl: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Epoll::waitEpoll( EpollData & epoll )
{
    epoll.nfds = epoll_wait( epoll.epoll_fd, epoll.events, MAX_EVENTS, TIMEOUT );
    if ( epoll.nfds == -1 )
    {
        if ( errno == EINTR )
            return ;
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_wait: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_wait: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}