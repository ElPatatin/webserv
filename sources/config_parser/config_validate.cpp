/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_validate.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:52:05 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/17 15:14:51 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parser.hpp"

static int  getEndOfBlock( ConfigFile & config );

bool    config_validate( ConfigFile config )
{
    int brackets_count = 0;
    int char_count = getEndOfBlock( config );
    if ( char_count == 0 )
        return ( false );
    int end_count = 0;

    for ( ConfigFile::iterator it = config.begin(); it != config.end(); ++it )
    {
        char    last_char = (*it)[ (*it).size() - 1 ];
        if ( last_char != ';' && last_char != '{' && last_char != '}' )
            return ( false );
        ++end_count;

        if ( (*it).find( "{" ) != std::string::npos )
            ++brackets_count;
        if ( (*it).find( "}" ) != std::string::npos )
            --brackets_count;
        if ( brackets_count < 0 )
            return ( false );
    }
    if ( brackets_count != 0 )
        return ( false );
    if ( end_count != char_count )
        return ( false );

    return ( true );
}

// Count all '{' and '}' and ';' and check if they are balanced
static int  getEndOfBlock( ConfigFile & config )
{
    int char_count = 0;
    for ( ConfigFile::iterator it = config.begin(); it != config.end(); ++it )
    {
        const std::string& line = *it;

        for ( std::string::const_iterator jt = line.begin(); jt != line.end(); ++jt )
        {
            switch ( *jt )
            {
                case '{':
                case '}':
                case ';':
                    ++char_count;
                    continue ;
                default:
                    continue ;
            }
        }
    }

    return ( char_count );
}