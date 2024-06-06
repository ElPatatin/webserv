/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/06 15:29:07 by cpeset-c         ###   ########.fr       */
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
        std::cerr << "\033[0;91m" << std::strerror(EINVAL) << "\033[0;39m" << std::endl;

    config = LoadConfig::loadConfig(ac, av);
    if (!LoadConfig::checkConfig(config))
        std::cerr << "\033[0;91m" << "Error: Configuration file is missing required keys" << "\033[0;39m" << std::endl;

    for (std::map<std::string, FieldInterface *>::const_iterator it = config.begin(); it != config.end(); ++it) {
        std::cout << it->first << " => ";
        it->second->printValue();
        std::cout << std::endl;

        delete it->second;
    }
    return ( 0 );
}