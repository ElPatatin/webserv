/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filesUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:12:48 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/21 16:19:29 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILESUTILS_HPP
# define FILESUTILS_HPP

# include "webserv.hpp"

std::fstream *  openFile( std::string config_path );
std::fstream *  deleteOpenFile( std::fstream * config_file );
void            closeFile( std::fstream * config_file );

#endif