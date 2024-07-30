/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets_comm.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:37:57 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/30 13:10:34 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"
#include "Http.hpp"

// ---------------------------------------------------------------------------

// Receive connection from client and parse request
std::string    CommunicationSockets::receiveConnection( const Data & data )
{
    char buffer[CHUNK_SIZE];
    std::memset(buffer, '\0', CHUNK_SIZE);

    std::string full_request;
    int content_length = 0;
    bool headers_received = false;
    int total_bytes_read = 0;
 
    while (true)
    {
        int ret = CommunicationSockets::waitTime( data, true );
        if (ret == -1)
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "select: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: select: " + std::string( std::strerror( errno ) ) );
        }
        else if (ret == 0)
        {
            LOG( WARNING ) << "select() timeout occurred, socket not ready for reading";
            continue ;
        }

        ssize_t bytes_read = Sockets::receiveConnection( data, buffer, static_cast< size_t >( CHUNK_SIZE ) - 1 );
        if (bytes_read == 0) { LOG( INFO ) << "Finished reading."; break ; }

        full_request.append( buffer, bytes_read );
        total_bytes_read += bytes_read;

        if ( !headers_received )
            headers_received = headersReceived( full_request, content_length );

        if ( !continueReceiving( full_request, headers_received, content_length, total_bytes_read ) )
            break ;
    }

    return ( full_request );
}

bool CommunicationSockets::headersReceived(const std::string &request, int &content_length)
{
    size_t header_end_pos = request.find( "\r\n\r\n" );
    if (header_end_pos != std::string::npos)
    {
        // Extract headers and find Content-Length
        std::string headers = request.substr( 0, header_end_pos + 4 );
        // LOG( WARNING ) << "Headers received: " << headers;
        Http::Request temp_http;
        try
        {
            temp_http = Http::parseRequest( request );
        }
        catch (std::exception & e)
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, e.what() );
            return ( false );
        }

        if (temp_http.headers.find("Content-Length") != temp_http.headers.end())
            content_length = ft::stoi(temp_http.headers["Content-Length"].second);

        return ( true );
    }

    return ( false );
}

bool    CommunicationSockets::continueReceiving
    (
        std::string full_request,
        bool headers_received,
        int content_length,
        int total_bytes_read
    )
{
    size_t header_end_pos = full_request.find("\r\n\r\n");
    if (headers_received && header_end_pos != std::string::npos)
    {
        size_t body_start_pos = header_end_pos + 4;
        size_t remaining_body_length = content_length - (total_bytes_read - body_start_pos);

        if ( remaining_body_length <= 0 )
            return ( false );
    }

    return ( true );
}

// Send response to client, chunked if necessary and wait for client to read.
void    CommunicationSockets::sendConnection( const Data & data )
{
    size_t total_length = data.response.length();
    const char* response_data = data.response.c_str();
    size_t bytes_sent = 0;

    int retry_count = 0;
    int max_retries = 10;

    while ( bytes_sent < total_length )
    {
        usleep( 1000 );
        size_t remainingLength = total_length - bytes_sent;
        size_t chunkLength = ( remainingLength > CHUNK_SIZE ) ? CHUNK_SIZE : remainingLength;

        int ret = CommunicationSockets::waitTime( data, false );
        if (ret == -1)
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, "select: " + std::string( std::strerror( errno ) ) );
            throw SocketException( "Error: select: " + std::string( std::strerror( errno ) ) );
        }
        else if (ret == 0)
        {
            LOG( WARNING ) << "select() timeout occurred, socket not ready for reading";
            if (++retry_count >= max_retries)
            {
                LOG(ERROR) << "Maximum retries reached, aborting send operation.";
                throw SocketException("Error: maximum retries reached, send operation aborted");
            }
            continue ;
        }

        try
        {
            ssize_t result = Sockets::sendConnection( data, response_data + bytes_sent, chunkLength );
            if ( result == 0 )
            {
                LOG( INFO ) << "Finished sending.";
                break ;
            }
            bytes_sent += result;
        }
        catch ( SocketException & e )
        {
            LOG( ERROR ) << ft::prettyPrint( __FUNCTION__, __LINE__, e.what() );
            return ;
        }
    }

    return ;
}

// Wait for socket to be ready for reading or writing
int    CommunicationSockets::waitTime( const Data & data, const bool & is_read )
{
    // Set up file descriptor set for select()
    fd_set waitfds;
    FD_ZERO( &waitfds );
    FD_SET( data.conn_fd, &waitfds );

    // Set up a timeout (e.g., 1 second)
    struct timeval  timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    // Wait until the socket is ready for reading
    if ( is_read )
        return ( select( data.conn_fd + 1, &waitfds, NULL, NULL, &timeout ) );
    else
        return ( select( data.conn_fd + 1, NULL, &waitfds, NULL, &timeout ) );
}