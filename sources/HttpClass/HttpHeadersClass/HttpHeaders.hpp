/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:00:56 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 11:34:11 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADERS_HPP
# define HTTPHEADERS_HPP

# include "common.hpp"
# include "Log.hpp"
# include "Exceptions.hpp"

# include <map>

# define LINE_END "\r\n"

typedef std::pair< std::string, std::string >   Header;
typedef std::map< std::string, Header >         Headers;

class HttpHeaders
{
    public:
        static std::string serializeHeader( const Headers & headers );
        static Headers deserializeHeader( const std::string & header );

    private:
        HttpHeaders( void );
        HttpHeaders( const HttpHeaders & src );
        HttpHeaders& operator=( const HttpHeaders & rhs );
        ~HttpHeaders( void );
};

#endif // HTTPHEADERS_HPP