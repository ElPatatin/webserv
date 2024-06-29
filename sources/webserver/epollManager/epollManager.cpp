/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epollManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:40:42 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/29 22:24:34 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

void    Epoll::createEpoll( Data & data, EpollData & epoll )
{
    UNUSED( data );
    epoll.epoll_fd = epoll_create( 1 );
    if ( epoll.epoll_fd == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_create: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_create1: " + std::string( std::strerror( errno ) ) );
    }
}

void    Epoll::addEpoll( Data & data, EpollData & epoll )
{
    epoll.event.data.fd = data.conn_fd;
    epoll.event.events = EPOLLIN | EPOLLET;

    if ( epoll_ctl( epoll.epoll_fd, EPOLL_CTL_ADD, data.conn_fd, &epoll.event ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_ctl: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_ctl: " + std::string( std::strerror( errno ) ) );
    }

    epoll.events = new epoll_event[ MAX_EVENTS ];
    return ;
}

void    Epoll::removeEpoll( Data & data, EpollData & epoll )
{
    if ( epoll_ctl( epoll.epoll_fd, EPOLL_CTL_DEL, data.conn_fd, &epoll.event ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_ctl: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_ctl: " + std::string( std::strerror( errno ) ) );
    }

    delete[] epoll.events;
    return ;
}

void    Epoll::updateEpoll( Data & data, EpollData & epoll )
{
    epoll.event.data.fd = data.conn_fd;
    epoll.event.events = EPOLLIN | EPOLLET;

    if ( epoll_ctl( epoll.epoll_fd, EPOLL_CTL_MOD, data.conn_fd, &epoll.event ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_ctl: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_ctl: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Epoll::waitEpoll( Data & data, EpollData & epoll )
{
    UNUSED( data );
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