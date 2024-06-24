/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 22:58:45 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/24 15:47:12 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <ctime>
# include <sys/stat.h>
# include <algorithm>

# include "utils.hpp"


# define LOG(x) Log().get(x)

# define LOGDIR "./logs/"
# define LOGFILE "webserver"
# define LOGEXT ".log"

// LOG LEVELS
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
        // CONSTRUCTOR & DESTRUCTOR
        // ========================
        Log();
        virtual ~Log(); // virtual destructor to allow derived classes to be destroyed properly

        // OPERATORS OVERLOAD
        // ==================
        std::ostringstream&     get( LogLevel level = INFO );
    
        // STATIC METHODS
        // ==============
        static LogLevel&        reportingLevel();
        static std::string      toString( LogLevel level );
        static LogLevel         fromString( const std::string & level );

    protected:
        // ATTRIBUTES
        // ==========
        std::ostringstream os;

    private:
        // COPY CONSTRUCTOR & ASSIGNATION OPERATOR
        // =======================================
        Log( const Log & );
        Log & operator=( const Log & );

        // ATTRIBUTES
        // ==========
        LogLevel messageLevel;

        // PRIVATE METHODS
        // ===============
        void                    writeLog( const std::string & message );
        static std::string      getTimeStr( void );
        std::string             makeLogFileName( void );
        static std::fstream     logFile;
};

#endif