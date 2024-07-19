/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:30:18 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/19 19:32:07 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
# define EPOLL_HPP

# include "common.hpp"
# include "ConfigData.hpp"
# include "Exceptions.hpp"
# include "Log.hpp"

# include <sys/epoll.h>
# include <fcntl.h>

# define MAX_EVENTS 64
# define TIMEOUT 300000

typedef struct epoll_event  EpollEvent;

typedef struct s_epoll
{
    int         epoll_fd;
    int         nfds;
    EpollEvent  event;
    EpollEvent  events[ MAX_EVENTS ];
}   EpollData;

namespace Epoll
{
    void    create_epoll( EpollData & epoll );
    void    add_epoll( EpollData & epoll, int sock, int events );
    void    wait_epoll( EpollData & epoll );
    void    close_epoll( EpollData & epoll );
    void    remove_epoll( EpollData & epoll, int sock );
    void    update_epoll( EpollData & epoll, int sock, int events );
}

#endif
