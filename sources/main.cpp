/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/18 17:33:31 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cerrno>
#include <exception>
#include "LoadConfig.hpp"
#include "Sock.hpp"

int main(int ac, char **av)
{
    std::map<std::string, FieldInterface *> config;
    
    if (ac < 1 || ac > 2)
        throw std::runtime_error("Usage: ./webserv [config_path - optional]");

    config = LoadConfig::loadConfig(ac, av);
    if (!LoadConfig::checkConfig(config))
        throw std::runtime_error("Missing required keys in configuration file");

    // int n_server;
    // n_server = config["server_names"]->getValue<t_vecstr >().size();
    // if (n_server == 0)
    // {
    //     n_server = 1;
    //     config["server_names"]->getValue<t_vecstr >().push_back("default");
    // }
    
    std::signal( SIGINT, Sock::handleSignal );
    std::signal( SIGTERM, Sock::handleSignal );
    std::signal( SIGQUIT, Sock::handleSignal );

    // for (int i = 0; i < n_server; i++)
    // {
        // Sock sock( AF_INET, SOCK_STREAM, 0, config["port"]->getValue<u_int16_t>(), config["server_names"]->getValue<t_vecstr>()[i] );
        Sock sock( AF_INET, SOCK_STREAM, 0, config["port"]->getValue<u_int16_t>(), config["host"]->getValue<std::string>() );
    // }
    
    return 0;
}