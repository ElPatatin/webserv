/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/06 17:04:12 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cerrno>
#include <exception>
#include "LoadConfig.hpp"

int main(int ac, char **av)
{
    std::map<std::string, FieldInterface *> config;
    
    if (ac < 1 || ac > 2)
        throw std::runtime_error("Usage: ./webserv [config_path - optional]");

    config = LoadConfig::loadConfig(ac, av);
    if (!LoadConfig::checkConfig(config))
        throw std::runtime_error("Missing required keys in configuration file");

    for (std::map<std::string, FieldInterface *>::const_iterator it = config.begin(); it != config.end(); ++it) {
        std::cout << it->first << " => ";
        it->second->printValue();
        std::cout << std::endl;

        delete it->second;
    }
    return ( 0 );
}