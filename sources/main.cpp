/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/17 17:58:02 by cpeset-c         ###   ########.fr       */
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


    // std::cout << "Number of server names: " << config["server_names"]->getValue<t_vecstr >().size() << std::endl;

    std::signal( SIGINT, Sock::handleSignal );
    std::signal( SIGTERM, Sock::handleSignal );
    std::signal( SIGQUIT, Sock::handleSignal );
    
    Sock sock( AF_INET, SOCK_STREAM, 0, config["port"]->getValue<u_int16_t>() );
    
    return 0;
}