/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver_utils.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 20:11:49 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/08/22 00:32:39 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"
#include "Http.hpp"

void    WebServer::handle_reading_request
(
    EpollData & epoll,
    ServerData * serverData,
    std::map< int, ServerData * > & connection_to_server_map,
    std::map< int, ServerData * >::iterator & connIt
)
{
    char buffer[ CHUNK_SIZE ];
    ssize_t bytes_read = Sockets::receiveConnection( serverData->data, buffer, CHUNK_SIZE );
    if (bytes_read == 0)
    {
        LOG(INFO) << "Connection closed by client";
        Sockets::closeConnection( serverData->data.conn_fd, __FUNCTION__, __LINE__ );
        connection_to_server_map.erase( connIt );
        WebServer::resetConnection( serverData );
        return ;
    }

    serverData->request_buffer += std::string( buffer, bytes_read );

    int content_length = 0;
    if ( WebServer::headersReceived( serverData->request_buffer, content_length ) )
    {
        // Check if the full request has been received
        if ( !WebServer::continueReceiving( serverData->request_buffer, content_length, serverData->request_buffer.size() ) )
        {
            serverData->state = PROCESSING_REQUEST;
            return ;
        }
    }

    // Update epoll event to continue reading
    Epoll::update_epoll( epoll, serverData->data.conn_fd, EPOLLIN | EPOLLET | EPOLLHUP | EPOLLERR );
    serverData->state = READING_REQUEST;
    return ;
}

void    WebServer::process_request( int event_fd, EpollData & epoll, ServerData * serverData )
{
    // Process the request
    Http::handleRequest( serverData->request_buffer, *serverData->config, serverData->data );
    serverData->response_buffer = serverData->data.response;

    // Transition to sending state
    serverData->state = SENDING_RESPONSE;
    Epoll::update_epoll( epoll, event_fd, EPOLLOUT | EPOLLET | EPOLLHUP | EPOLLERR );
    serverData->bytes_sent = 0;
}

void    WebServer::handle_sending_response
(
    EpollData & epoll,
    ServerData * serverData,
    std::map< int, ServerData * > & connection_to_server_map,
    std::map< int, ServerData * >::iterator & connIt
)
{
    // Send the response by chunks of CHUNK_SIZE
    size_t bytes_to_send = serverData->response_buffer.size() - serverData->bytes_sent;
    if ( bytes_to_send > CHUNK_SIZE )
        bytes_to_send = CHUNK_SIZE;

    ssize_t bytes_sent = Sockets::sendConnection( serverData->data, serverData->response_buffer.c_str() + serverData->bytes_sent, bytes_to_send );
    if ( bytes_sent == 0 )
    {
        LOG( ERROR ) << "Failed to send response";
        Sockets::closeConnection( serverData->data.conn_fd, __FUNCTION__, __LINE__ );
        connection_to_server_map.erase( connIt );
        WebServer::resetConnection( serverData );
        return ;
    }

    serverData->bytes_sent += bytes_sent;
    if ( serverData->bytes_sent == serverData->response_buffer.size() )
    {
        serverData->state = CLOSING;
        return ;
    }

    Epoll::update_epoll( epoll, serverData->data.conn_fd, EPOLLOUT | EPOLLET | EPOLLHUP | EPOLLERR );
    serverData->state = SENDING_RESPONSE;
    return ;
}

void    WebServer::close_connection
(
    int event_fd,
    std::map< int, ServerData * > & connection_to_server_map,
    std::map< int, ServerData * >::iterator & connIt,
    ServerData * serverData
)
{
    Sockets::closeConnection( event_fd, __FUNCTION__, __LINE__ );
    connection_to_server_map.erase( connIt );
    WebServer::resetConnection( serverData );
    return ;
}

void    WebServer::handle_bad_request
(
    ServerData * & serverData,
    EpollData & epoll,
    std::map< int, ServerData * >::iterator & connIt,
    std::map < int, ServerData * > & connection_to_server_map
)
{
    UNUSED( epoll );
    LOG( INFO ) << "Bad request, closing connection";
    // Send a 400 Bad Request response
    // TODO: Implement this
    Sockets::closeConnection( serverData->data.conn_fd, __FUNCTION__, __LINE__ );
    connection_to_server_map.erase( connIt );
    WebServer::resetConnection( serverData );
    return ;
}

void    WebServer::resetConnection( ServerData * & serverData )
{
    // Reset the server data for the next connection
    serverData->state = READING_REQUEST;
    serverData->request_buffer.clear();
    serverData->response_buffer.clear();
    serverData->bytes_sent = 0;
}

void    WebServer::add_listening_sockets_to_epoll( Servers & servers, EpollData & epoll )
{
    // Add all listening sockets to epoll
    for ( Servers::iterator it = servers.begin(); it != servers.end(); ++it )
    {
        LOG( INFO ) << "Server listening on port " << it->second.config->getPort();
        Epoll::add_epoll( epoll, it->second.data.listen_sock, EPOLLIN );
    }
}

bool WebServer::headersReceived(const std::string &request, int &content_length)
{
    size_t header_end_pos = request.find( "\r\n\r\n" );
    if (header_end_pos != std::string::npos)
    {
        // Extract headers and find Content-Length
        std::string headers = request.substr( 0, header_end_pos + 4 );
        // LOG( WARNING ) << "Headers received: " << headers;
        Headers temp_http;
        try { temp_http = HttpRequestParser::deserializeHeader( headers ); }
        catch (std::exception & e) { LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, e.what() ); return ( false ); }

        if (temp_http.find( "Content-Length" ) != temp_http.end())
        {
            content_length = ft::stoi( temp_http[ "Content-Length" ].second );
            return ( true );
        }


        return ( true );
    }

    return ( false );
}

bool    WebServer::continueReceiving( std::string full_request, int content_length, int total_bytes_read )
{
    size_t header_end_pos = full_request.find("\r\n\r\n");
    if (header_end_pos != std::string::npos)
    {
        size_t body_start_pos = header_end_pos + 4;
        size_t remaining_body_length = content_length - (total_bytes_read - body_start_pos);

        if ( remaining_body_length <= 0 )
            return ( false );
    }

    return ( true );
}
