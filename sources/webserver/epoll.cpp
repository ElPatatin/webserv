/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:40:42 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/19 19:41:57 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "epoll.hpp"

void    Epoll::create_epoll( EpollData & epoll )
{
    epoll.epoll_fd = epoll_create( 1 );
    if ( epoll.epoll_fd == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_create: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_create1: " + std::string( std::strerror( errno ) ) );
    }
}

void    Epoll::add_epoll( EpollData & epoll, int sock, int events )
{
    LOG( DEBUG ) << "Adding epoll";
    epoll.event.data.fd = sock;
    epoll.event.events = events;

    if ( epoll_ctl( epoll.epoll_fd, EPOLL_CTL_ADD, sock, &epoll.event ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_ctl: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_ctl: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Epoll::wait_epoll( EpollData & epoll )
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

void    Epoll::close_epoll( EpollData & epoll )
{
    LOG( DEBUG ) << "Closing epoll";
    close( epoll.epoll_fd );
    return ;
}

void    Epoll::remove_epoll( EpollData & epoll, int sock )
{
    LOG( DEBUG ) << "Removing epoll";
    if ( epoll_ctl( epoll.epoll_fd, EPOLL_CTL_DEL, sock, &epoll.event ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_ctl: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_ctl: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}

void    Epoll::update_epoll( EpollData & epoll, int sock, int events )
{
    LOG( DEBUG ) << "Modifying epoll";
    epoll.event.data.fd = sock;
    epoll.event.events = events;

    if ( epoll_ctl( epoll.epoll_fd, EPOLL_CTL_MOD, sock, &epoll.event ) == -1 )
    {
        LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "epoll_ctl: " + std::string( std::strerror( errno ) ) );
        throw EpollException( "Error: epoll_ctl: " + std::string( std::strerror( errno ) ) );
    }

    return ;
}