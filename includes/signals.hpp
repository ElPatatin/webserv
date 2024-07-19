/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barce.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:04:15 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/19 19:06:13 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_HPP
# define SIGNALS_HPP

# include "common.hpp"
# include <csignal>

/**
 * @brief This namespace contains the signal status
 * 
 * This namespace contains the signal status, which is used to handle
 * the signal in the main loop of the webserver.
 */
namespace g_signal { extern volatile sig_atomic_t g_signal_status; }

/**
 * @brief This function handles the signals
 * 
 * This function handles the signals that are sent to the webserver.
 * 
 * @param signum The signal number
 * @return void
 */
void    signalHandler( int signum );

#endif