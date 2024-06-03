/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:26:50 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/03 18:30:16 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
int main(int ac, char **av)
{
    (void)av;
    if (ac != 2)
    {
        std::cout << "Error: Wrong number of arguments" << std::endl;
        return ( 1 );
    }

    return ( 0 );
}