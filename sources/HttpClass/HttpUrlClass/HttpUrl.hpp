/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpUrl.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 13:02:57 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/20 17:12:20 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPURL_HPP
# define HTTPURL_HPP

# include "common.hpp"
# include "Log.hpp"
# include "Exceptions.hpp"

class HttpUrl
{
    public:
        HttpUrl( void );
        HttpUrl( const HttpUrl & src );
        HttpUrl& operator=( const HttpUrl & rhs );
        ~HttpUrl( void );

        static std::string decode( const std::string & url );
        static std::string encode( const std::string & url );
};

#endif // HTTPURL_HPP
