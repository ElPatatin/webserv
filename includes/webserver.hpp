/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:31:58 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/21 21:40:51 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "sockets.hpp"
# include "epoll.hpp"
# include "signals.hpp"

enum ConnectionState
{
    READING_REQUEST,
    PROCESSING_REQUEST,
    SENDING_RESPONSE,
    CLOSING
};

struct ServerData
{
    Addrs       addrs;
    Data        data;
    ConfigData * config;

    ConnectionState state;
    std::string request_buffer;
    std::string response_buffer;
    size_t      bytes_sent;

    ServerData() : state( READING_REQUEST ), bytes_sent(0) {}
};

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
    void    handle_existing_connection( int event_fd, uint32_t events, EpollData & epoll, std::map < int, ServerData * > & connection_to_server_map );
    void    handle_bad_request( ServerData * & serverData, EpollData & epoll, std::map< int, ServerData * >::iterator & connIt, std::map < int, ServerData * > & connection_to_server_map );
    void    add_listening_sockets_to_epoll( Servers & servers, EpollData & epoll );
    void    resetConnection( ServerData * & serverData );
    void    handle_reading_request( EpollData & epoll, ServerData * serverData, std::map< int, ServerData * > & connection_to_server_map, std::map< int, ServerData * >::iterator & connIt );
    void    process_request( int event_fd, EpollData & epoll, ServerData * serverData );
    void    handle_sending_response( EpollData & epoll, ServerData * serverData, std::map< int, ServerData * > & connection_to_server_map, std::map< int, ServerData * >::iterator & connIt );
    void    close_connection( int event_fd, std::map< int, ServerData * > & connection_to_server_map, std::map< int, ServerData * >::iterator & connIt, ServerData * serverData );

    bool    headersReceived( const std::string & request, int & content_length );
    bool    continueReceiving( std::string full_request, int content_length, int total_bytes_read );
}

#endif