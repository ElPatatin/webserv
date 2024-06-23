/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 22:58:45 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 16:10:19 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <time.h>
# include <sys/stat.h>
# include <dirent.h>

# include "fileUtils.hpp"


# define LOG(x) Log().get(x)

# define LOGFILE "webserver.log"

enum LogLevel
{
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
    DEBUG1,
    DEBUG2,
    DEBUG3
};


/**
 * @brief Class to log messages
 * 
 * @see https://en.cppreference.com/w/cpp/language/raii
 * This class follows the RAII pattern:
 * - The constructor opens the log file
 * - The destructor writes the message in the log file
 * 
 * This class is used to log messages in a file.
*/
class Log
{
    public:
        Log();
        virtual ~Log();

        std::ostringstream& get( LogLevel level = INFO );
    
        static LogLevel& reportingLevel();
        static std::string toString( LogLevel level );
        static LogLevel fromString( const std::string & level );

    protected:
        std::ostringstream os;

    private:
        Log( const Log & );
        Log & operator=( const Log & );
        LogLevel messageLevel;

        void writeLog( const std::string & message );
        static std::string getTimeStr( void );
        static std::fstream logFile;
};

#endif