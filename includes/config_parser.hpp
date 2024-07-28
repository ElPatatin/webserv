/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:22:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/07/28 10:53:45 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "utils.hpp"
# include "Exceptions.hpp"
# include "ConfigData.hpp"
# include "Log.hpp"

# define DEFAULT_CONF_PATH "./configuration/default.conf"

typedef enum e_dict_size
{
    BYTE,
    KILO = 'K',
    MEGA = 'M',
}   DictSize;
typedef struct s_info_server
{
    size_t  line_num;
    bool    is_virtual;
}           InfoServer;

typedef std::vector < std::string > ConfigFile;

void    config_parser( int ac, char **av, Cluster & cluster );

ConfigFile  conf_get( std::fstream * config_file );

bool    config_validate( ConfigFile config );

size_t config_server_count( ConfigFile config );

void    config_load( const ConfigFile & config, ConfigData * config_data, size_t n_servers );

bool    config_load_validate( ConfigData * & config_data );

namespace ConfigLoad
{
    std::string config_load_line( std::string * line, std::string error_message );
    bool config_load_port( std::string line, ConfigData & config_data );
    bool config_load_server_name( std::string line, ConfigData & config_data );
    bool config_load_error_page( std::string line, ConfigData & config_data );
    bool config_load_client_max_body_size( std::string & line, ConfigData & config_data );
    bool config_load_locations( const ConfigFile & config_file, std::string line, size_t line_num, ConfigData & config_data );
    bool config_load_redirect( std::string endpoint, std::string line, ConfigData & config_data );
}

#endif
