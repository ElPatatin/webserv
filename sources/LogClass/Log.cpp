/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpeset-c <cpeset-c@student.42barcel.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 22:58:47 by cpeset-c          #+#    #+#             */
/*   Updated: 2024/06/23 16:09:59 by cpeset-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

std::fstream Log::logFile;

Log::Log( void )
    : messageLevel( INFO )
{
    if ( !logFile.is_open() )
    {
        logFile.open( LOGFILE, std::ios::out | std::ios::app );
        if ( logFile.fail() )
            std::cerr << "Error: " << LOGFILE << " could not be opened" << std::endl;
    }
    return ;
}

Log::~Log( void )
{
    if ( os.str().empty() )
        return ;
    writeLog( os.str() );
    if ( logFile.is_open() )
        logFile.close();
    return ;
}

std::ostringstream& Log::get( LogLevel level )
{
    messageLevel = level;
    os << "- [" << toString( messageLevel ) << "] - ";
    return ( os );
}

LogLevel& Log::reportingLevel( void )
{
    static LogLevel reportingLevel = DEBUG3;
    return reportingLevel;
}

std::string Log::toString( LogLevel level )
{
    static const char * const buffer[] = { "INFO", "WARNING", "ERROR", "CRITICAL", "DEBUG1", "DEBUG2", "DEBUG3" };
    return ( buffer[ level ] );
}

LogLevel Log::fromString( const std::string & level )
{
    if ( level == "DEBUG3" ) return ( DEBUG3 );
    if ( level == "DEBUG2" ) return ( DEBUG2 );
    if ( level == "DEBUG1" ) return ( DEBUG1 );
    if ( level == "CRITICAL" ) return ( CRITICAL );
    if ( level == "ERROR" ) return ( ERROR );
    if ( level == "WARNING" ) return ( WARNING );
    if ( level == "INFO" ) return ( INFO );
    Log().get( CRITICAL ) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return ( INFO );
}

void Log::writeLog( const std::string & message )
{
    if ( messageLevel <= reportingLevel() )
        logFile << getTimeStr() << " " << message << std::endl;

    logFile.flush();
    return ;
}

std::string Log::getTimeStr( void )
{
    time_t t = time( NULL );
    char buffer[ 100 ];
    struct tm * tm_info = localtime( &t );

    strftime( buffer, sizeof( buffer ), "%d-%m-%Y %H:%M:%S", tm_info );
    return ( buffer );
}