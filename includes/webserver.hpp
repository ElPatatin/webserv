/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:31:58 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/26 19:37:32 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "sockets.hpp"
# include "epoll.hpp"
# include "signals.hpp"

typedef struct s_server_data {
    Addrs addrs;
    Data data;
    ConfigData * config;
}   ServerData;

typedef std::map < int, ServerData > Servers;

/**
 * @brief Function to create a web server
 * 
 * @param cluster
 * @return void
 */
void    webserver( Cluster & cluster );

namespace WebServer
{
    bool start_server( ConfigData & config, Servers & servers );
    bool run_server( Servers & servers );
    bool stop_server( Servers & servers );

    bool    run_server_loop( Servers & servers, EpollData & epoll, std::map < int, ServerData * > & connection_to_server_map );
    void    handle_new_connection( int event_fd, Servers & servers, EpollData & epoll, std::map < int, ServerData * > & connection_to_server_map );
    void    handle_existing_connection( int event_fd, std::map < int, ServerData * > & connection_to_server_map );
    void    add_listening_sockets_to_epoll( Servers & servers, EpollData & epoll );
}

#endif