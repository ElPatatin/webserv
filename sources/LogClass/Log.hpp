/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 22:58:45 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/30 13:00:35 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# include <ctime>
# include <sys/stat.h>
# include <algorithm>

# include "utils.hpp"


# define LOG(x) Log().get(x)

# define LOGFILE "webserver.log"

// LOG LEVELS
enum LogLevel
{
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
    DEBUG
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
        static std::fstream     logFile;
};

#endif