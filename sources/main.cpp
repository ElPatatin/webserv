/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/08 18:08:20 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cerrno>
#include <exception>
#include "LoadConfig.hpp"

void test(std::map<std::string, FieldInterface *> config);


int main(int ac, char **av)
{
    std::map<std::string, FieldInterface *> config;
    
    if (ac < 1 || ac > 2)
        throw std::runtime_error("Usage: ./webserv [config_path - optional]");

    config = LoadConfig::loadConfig(ac, av);
    if (!LoadConfig::checkConfig(config))
        throw std::runtime_error("Missing required keys in configuration file");

    test(config);
}