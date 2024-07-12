/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:40:49 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/27 19:04:34 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"
#include "colors.hpp"

void    signalHandler( int signum )
{
    g_signal::g_signal_status = false;

    std::cout << "\n\033[A\033[K";
    if ( signum == SIGINT )
        std::cout << ORANGE << "SIGINT received" << RESET << std::endl;
    else if ( signum == SIGQUIT )
        std::cout << ORANGE << "SIGQUIT received" << RESET << std::endl;

    return ;
}
