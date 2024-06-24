/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:12:48 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/24 15:27:01 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILESUTILS_HPP
# define FILESUTILS_HPP

# include "webserv.hpp"

namespace ft
{
    std::fstream *  openFile( std::string file_path, std::ios::openmode _mode );
    void            closeFile( std::fstream * _file );
    std::fstream *  deleteOpenFile( std::fstream * _file );
    void            trim( std::string& str );
    std::string&    rtrim( std::string & str );
    std::vector< std::string > split( const std::string & str, char delimiter );
}

#endif