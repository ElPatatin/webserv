/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/06 12:34:36 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <cerrno>
#include <exception>

int main(int ac, char **av)
{
    (void)av;
    if (ac < 1 || ac > 2)
        std::cerr << "\033[0;91m" << std::strerror(EINVAL) << "\033[0;39m" << std::endl;
    // loadConfig(ac, av);z
    return ( 0 );
}