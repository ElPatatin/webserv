/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets_comm.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:37:57 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/19 19:41:54 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"
#include "http.hpp"

void    CommunicationSockets::receiveConnection( Data & data, ConfigData & config )
{
    const int buffer_size = 8192;
    char buffer[buffer_size];
    std::memset(buffer, '\0', buffer_size);

    std::string full_request;
    int content_length = 0;
    bool headers_received = false;
    int total_bytes_read = 0;

    while (true)
    {
        // Set up file descriptor set for select()
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(data.conn_fd, &readfds);

        // Set up a timeout (e.g., 1 second)
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // Wait until the socket is ready for reading
        int ret = select(data.conn_fd + 1, &readfds, NULL, NULL, &timeout);

        if (ret == -1)
        {
            LOG(ERROR) << ft::prettyPrint(__FUNCTION__, __LINE__, "select: " + std::string(std::strerror(errno)));
            throw SocketException("Error: select: " + std::string(std::strerror(errno)));
        }
        else if (ret == 0)
        {
            // Timeout occurred; handle as needed
            LOG(WARNING) << "select() timeout occurred, socket not ready for reading";
            continue; // Retry or handle timeout
        }

        // Socket is ready for reading, proceed with recv()
        int bytes_read = recv(data.conn_fd, buffer, buffer_size - 1, 0);
        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';
            full_request.append(buffer, bytes_read);
            total_bytes_read += bytes_read;

            if (!headers_received)
                headers_received = headersReceived(full_request, content_length);

            size_t header_end_pos = full_request.find("\r\n\r\n");
            if (headers_received && header_end_pos != std::string::npos)
            {
                size_t body_start_pos = header_end_pos + 4;
                size_t remaining_body_length = content_length - (total_bytes_read - body_start_pos);

                if (remaining_body_length <= 0)
                    break; // Fully received headers and body
            }
        }
        else if (bytes_read == 0)
        {
            // Connection closed
            break;
        }
        else
        {
            LOG(ERROR) << ft::prettyPrint(__FUNCTION__, __LINE__, "recv: " + std::string(std::strerror(errno)));
            throw SocketException("Error: recv: " + std::string(std::strerror(errno)));
        }
    }

    HttpData http = HttpRequests::parseRequest(full_request);
    Http::httpRequest(http, data, config);
    return;
}

bool CommunicationSockets::headersReceived(const std::string &request, int &content_length)
{
    size_t header_end_pos = request.find("\r\n\r\n");
    if (header_end_pos != std::string::npos)
    {
        // Extract headers and find Content-Length
        std::string headers = request.substr(0, header_end_pos + 4);
        HttpData temp_http = HttpRequests::parseRequest(headers);

        if (temp_http.headers.find("Content-Length") != temp_http.headers.end())
            content_length = ft::stoi(temp_http.headers["Content-Length"].second);

        return true;
    }

    return false;
}

void    CommunicationSockets::sendConnection( Data & data )
{
  const size_t CHUNK_SIZE = 4096; // Define the chunk size (e.g., 4 KB)
    size_t totalLength = data.response.length();
    const char* responseData = data.response.c_str();
    size_t bytesSent = 0;

    while (bytesSent < totalLength)
    {
        size_t remainingLength = totalLength - bytesSent;
        size_t chunkLength = (remainingLength > CHUNK_SIZE) ? CHUNK_SIZE : remainingLength;

        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(data.conn_fd, &writefds);

        // Set up a timeout (e.g., 1 second)
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ret = select(data.conn_fd + 1, NULL, &writefds, NULL, &timeout);

        if (ret == -1)
        {
            LOG(ERROR) << ft::prettyPrint(__FUNCTION__, __LINE__, "select: " + std::string(std::strerror(errno)));
            throw SocketException("Error: select: " + std::string(std::strerror(errno)));
        }
        else if (ret == 0)
        {
            // Timeout occurred; handle as needed
            LOG(WARNING) << "select() timeout occurred, socket not ready for writing";
            continue; // Retry or handle timeout
        }

        // Send data
        ssize_t result = send(data.conn_fd, responseData + bytesSent, chunkLength, 0);
        if (result == -1)
        {
            LOG(ERROR) << ft::prettyPrint(__FUNCTION__, __LINE__, "send: " + std::string(std::strerror(errno)));
            throw SocketException("Error: send: " + std::string(std::strerror(errno)));
        }
        
        bytesSent += result;
    }
}
