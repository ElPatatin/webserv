/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:12:48 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 14:44:47 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILESUTILS_HPP
# define FILESUTILS_HPP

# include "webserv.hpp"

std::fstream *  openFile( std::string config_path );
void            closeFile( std::fstream * config_file );
std::fstream *  deleteOpenFile( std::fstream * config_file );
void            trim( std::string& str );
std::string&    rtrim( std::string & str );
std::vector< std::string > split( const std::string & str, char delimiter );

#endif