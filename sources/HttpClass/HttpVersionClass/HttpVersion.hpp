/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersion.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:02:53 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/23 15:39:37 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERSION_HPP
# define VERSION_HPP

# include "common.hpp"
# include "Log.hpp"
# include "Exceptions.hpp"

# define HTTP_VERSION_1_0 "HTTP/1.0"
# define HTTP_VERSION_1_1 "HTTP/1.1"
# define HTTP_VERSION_2_0 "HTTP/2.0"

# define HTTP_1_0_HASH 0x1b0b7d
# define HTTP_1_1_HASH 0x1b0b7e
# define HTTP_2_0_HASH 0x1b0b7f

class HttpVersion
{
    public:
        typedef enum e_type
        {
            HTTP_1_0,
            HTTP_1_1,
            HTTP_2_0
        }   Version;

        static std::string toString( const size_t & hash );
        static Version fromString( const std::string & version );

    protected:
        HttpVersion( void );
        HttpVersion( const HttpVersion & src );
        HttpVersion& operator=( const HttpVersion & rhs );
        ~HttpVersion( void );

        static unsigned int hash( const std::string & str );
};

#endif // VERSION_HPP
