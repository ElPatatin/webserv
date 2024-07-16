/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:22:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/16 15:04:10 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "utils.hpp"
# include "Exceptions.hpp"
# include "ConfigData.hpp"
# include "Log.hpp"

# define DEFAULT_CONF_PATH "./configuration/default.conf"

typedef std::vector < std::string > ConfigFile;

void    config_parser( int ac, char **av );

ConfigFile  conf_get( std::fstream * config_file );

bool    config_validate( ConfigFile config );

size_t config_server_count( ConfigFile config );

void    config_load( const ConfigFile & config, ConfigData * & config_data );

bool    config_load_validate( ConfigData * & config_data );


#endif
