/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:00:56 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/20 17:17:42 by cpeset-c         ###   ########.fr       */
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
        HttpHeaders( void );
        HttpHeaders( const HttpHeaders & src );
        HttpHeaders& operator=( const HttpHeaders & rhs );
        ~HttpHeaders( void );

        std::string serializeHeader( const Headers & headers ) const;
        Headers deserializeHeader( const std::string & header ) const;
};

#endif // HTTPHEADERS_HPP