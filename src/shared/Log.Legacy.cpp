/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2016 AscEmu Team <http://www.ascemu.org/>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include "Config/Config.h"
#include "Log.hpp"
#include "Util.hpp"
#include <cstdarg>
#include <string>
#include <algorithm>

std::string FormatOutputString(const char* Prefix, const char* Description, bool useTimeStamp)
{
    char p[MAX_PATH];
    p[0] = 0;
    strcat(p, Prefix);
    strcat(p, "/");
    strcat(p, Description);
    if(useTimeStamp)
    {
        std::string current_date_time = Util::GetCurrentDateTimeString();
        //replace time seperator with valid character for file name
        std::replace(current_date_time.begin(), current_date_time.end(), ':', '-');
        char ftime[100];
        snprintf(ftime, 100, "-%s", current_date_time.c_str());
        strcat(p, ftime);
    }

    strcat(p, ".log");
    return std::string(p);
}

createFileSingleton(oLog);
initialiseSingleton(WorldLog);

SERVER_DECL time_t UNIXTIME;
SERVER_DECL tm g_localTime;

void oLog::SetColor(int color)
{
#ifndef _WIN32
    static const char* colorstrings[TBLUE+1] = {
   "",
    "\033[22;31m",
    "\033[22;32m",
    "\033[01;33m",
    //"\033[22;37m",
    "\033[0m",
    "\033[01;37m",
    "\033[1;34m",
  };
    fputs(colorstrings[color], stdout);
#else
    SetConsoleTextAttribute(stdout_handle, (WORD)color);
#endif
}

void oLog::outFile(FILE* file, char* msg, const char* source)
{
    std::string current_time = "[" + Util::GetCurrentTimeString() + "] ";
    if(source != NULL)
    {
        fprintf(file, "%s %s: %s\n", current_time.c_str(), source, msg);
        //printf("%s %s: %s\n", time_buffer, source, msg);
    }
    else
    {
        fprintf(file, "%s %s\n", current_time.c_str(), msg);
        //printf("%s %s\n", time_buffer, msg);
    }
}

/// Prints text to file without showing it to the user. Used for the startup banner.
void oLog::outFileSilent(FILE* file, char* msg, const char* source)
{
    std::string current_time = "[" + Util::GetCurrentTimeString() + "] ";
    if(source != NULL)
    {
        fprintf(file, "%s %s: %s\n", current_time.c_str(), source, msg);
        // Don't use printf to prevent text from being shown in the console output.
    }
    else
    {
        fprintf(file, "%s %s\n", current_time.c_str(), msg);
        // Don't use printf to prevent text from being shown in the console output.
    }
}

void oLog::outString(const char* str, ...)
{
    if(m_normalFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, str);
    vsnprintf(buf, 32768, str, ap);
    va_end(ap);
    SetColor(TNORMAL);
    std::cout << buf << std::endl;
    outFile(m_normalFile, buf);
}

void oLog::outError(const char* err, ...)
{
    if(m_errorFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, err);
    vsnprintf(buf, 32768, err, ap);
    va_end(ap);
    SetColor(TRED);
    std::cout << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_errorFile, buf);
}

/// Writes into the error log without giving console output. Used for the startup banner.
void oLog::outErrorSilent(const char* err, ...)
{
    if(m_errorFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, err);
    vsnprintf(buf, 32768, err, ap);
    va_end(ap);

    outFileSilent(m_errorFile, buf); // This uses a function that won't give console output.
}

void oLog::outBasic(const char* str, ...)
{
    if(m_normalFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, str);
    vsnprintf(buf, 32768, str, ap);
    va_end(ap);
    SetColor(TBLUE);
    std::cout << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_normalFile, buf);
}

void oLog::outDetail(const char* str, ...)
{
    if(m_fileLogLevel < LOG_LEVEL_DETAIL || m_normalFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, str);
    vsnprintf(buf, 32768, str, ap);
    va_end(ap);
    SetColor(TWHITE);
    std::cout << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_normalFile, buf);
}

void oLog::outDebug(const char* str, ...)
{
    if(m_fileLogLevel < LOG_LEVEL_DEBUG || m_errorFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, str);
    vsnprintf(buf, 32768, str, ap);
    va_end(ap);
    SetColor(TYELLOW);
    std::cout << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_errorFile, buf);
}

void oLog::logBasic(const char* file, int line, const char* fncname, const char* msg, ...)
{
    if(m_normalFile == NULL)
        return;

    char buf[ 32768 ];
    char message[ 32768 ];

    snprintf(message, 32768, "[BSC] %s %s", fncname, msg);
    //snprintf(message, 32768, "[BSC] %s:%d %s %s", file, line, fncname, msg);
    va_list ap;

    va_start(ap, msg);
    vsnprintf(buf, 32768, message, ap);
    va_end(ap);
    SetColor(TWHITE);
    std::cout << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_normalFile, buf);
}

void oLog::logDetail(const char* file, int line, const char* fncname, const char* msg, ...)
{
    if((m_fileLogLevel < LOG_LEVEL_DETAIL) || (m_normalFile == NULL))
        return;

    char buf[ 32768 ];
    char message[ 32768 ];

    snprintf(message, 32768, "[DTL] %s %s", fncname, msg);
    //snprintf(message, 32768, "[DTL] %s:%d %s %s", file, line, fncname, msg);
    va_list ap;

    va_start(ap, msg);
    vsnprintf(buf, 32768, message, ap);
    va_end(ap);
    SetColor(TWHITE);
    std::cout << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_normalFile, buf);
}

void oLog::logError(const char* file, int line, const char* fncname, const char* msg, ...)
{
    if(m_errorFile == NULL)
        return;

    char buf[ 32768 ];
    char message[ 32768 ];

    snprintf(message, 32768, "[ERR] %s %s", fncname, msg);
    //snprintf(message, 32768, "[ERR] %s:%d %s %s", file, line, fncname, msg);
    va_list ap;

    va_start(ap, msg);
    vsnprintf(buf, 32768, message, ap);
    va_end(ap);
    SetColor(TRED);
    std::cout << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_errorFile, buf);
}

void oLog::logDebug(const char* file, int line, const char* fncname, const char* msg, ...)
{
    if((m_fileLogLevel < LOG_LEVEL_DEBUG) || (m_errorFile == NULL))
        return;

    char buf[ 32768 ];
    char message[ 32768 ];

    snprintf(message, 32768, "[DBG] %s %s", fncname, msg);
    //snprintf(message, 32768, "[DBG] %s:%d %s %s", file, line, fncname, msg);
    va_list ap;

    va_start(ap, msg);
    vsnprintf(buf, 32768, message, ap);
    va_end(ap);

    outFile(m_errorFile, buf);
}

//old NGLog.h methods
void oLog::Notice(const char* source, const char* format, ...)
{
    if(m_fileLogLevel < LOG_LEVEL_DETAIL || m_normalFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, format, ap);
    va_end(ap);
    SetColor(TGREEN);
    std::cout << source << ": " << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_normalFile, buf, source);
}

void oLog::Warning(const char* source, const char* format, ...)
{
    if(m_fileLogLevel < LOG_LEVEL_DETAIL || m_normalFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, format, ap);
    va_end(ap);
    SetColor(TWHITE);
    std::cout << source << ": " << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_normalFile, buf, source);
}

void oLog::Success(const char* source, const char* format, ...)
{
    if(m_normalFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, format, ap);
    va_end(ap);
    SetColor(TNORMAL);
    std::cout << source << ": " << buf << std::endl;
    outFile(m_normalFile, buf, source);
}

void oLog::Error(const char* source, const char* format, ...)
{
    if(m_errorFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, format, ap);
    va_end(ap);
    SetColor(TRED);
    std::cout << source << ": " << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_errorFile, buf, source);
}

void oLog::Debug(const char* source, const char* format, ...)
{
    if (m_fileLogLevel < LOG_LEVEL_DEBUG || m_errorFile == NULL)
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, format, ap);
    va_end(ap);
    SetColor(TYELLOW);
    std::cout << source << ": " << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_errorFile, buf, source);
}

void oLog::DebugFlag(LogFlags log_flags, const char* format, ...)
{
    if (m_fileLogLevel < LOG_LEVEL_DEBUG || m_errorFile == NULL)
        return;

    if (!(mDebugFlags & log_flags))
        return;

    char buf[32768];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, 32768, format, ap);
    va_end(ap);
    SetColor(GetColorForDebugFlag(log_flags));
    std::cout << buf << std::endl;
    SetColor(TNORMAL);
    outFile(m_errorFile, buf);
}

int oLog::GetColorForDebugFlag(LogFlags log_flags)
{
    switch (log_flags)
    {
        case LF_MAP:
        case LF_MAP_CELL:
        case LF_VMAP:
        case LF_MMAP:
            return TBLUE;
        case LF_OPCODE:
            return TWHITE;
        case LF_SPELL:
        case LF_AURA:
        case LF_SPELL_EFF:
        case LF_AURA_EFF:
            return TPURPLE;
        case LF_SCRIPT_MGR:
        case LF_DB_TABLES:
            return TYELLOW;
        default:
            return TNORMAL;
    }

}

void oLog::LargeErrorMessage(const char* source, ...)
{
    std::vector<char*> lines;
    char* pointer;
    va_list ap;
    va_start(ap, source);

    pointer = const_cast<char*>(source);
    lines.push_back(pointer);

    size_t i, j, k;
    pointer = va_arg(ap, char*);
    while(pointer != NULL)
    {
        lines.push_back(pointer);
        pointer = va_arg(ap, char*);
    }

    va_end(ap);

    outError("*********************************************************************");
    outError("*                        MAJOR ERROR/WARNING                        *");
    outError("*                        ===================                        *");

    for(std::vector<char*>::iterator itr = lines.begin(); itr != lines.end(); ++itr)
    {
        std::stringstream sstext;
        i = strlen(*itr);
        j = (i <= 65) ? 65 - i : 0;
        sstext << "* " << *itr;
        for(k = 0; k < j; ++k)
        {
            sstext << " ";
        }

        sstext << " *";
        outError(sstext.str().c_str());
    }

    outError("*********************************************************************");
}

void oLog::Init(int32 fileLogLevel, LogType logType)
{
#ifdef _WIN32
    stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    SetFileLoggingLevel(fileLogLevel);

    const char* logNormalFilename = NULL, *logErrorFilename = NULL;
    switch(logType)
    {
        case LOGON_LOG:
            {
                logNormalFilename = "logon-normal.log";
                logErrorFilename = "logon-error.log";
                break;
            }
        case WORLD_LOG:
            {
                logNormalFilename = "world-normal.log";
                logErrorFilename = "world-error.log";
                break;
            }
    }

    std::string current_date_time = Util::GetCurrentDateTimeString();

    m_normalFile = fopen(logNormalFilename, "a");
    if (m_normalFile == nullptr)
    {
        fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logNormalFilename, strerror(errno));
    }
    else
    {
        outBasic("=================[%s]=================", current_date_time.c_str());
    }

    m_errorFile = fopen(logErrorFilename, "a");
    if (m_errorFile == nullptr)
    {
        fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logErrorFilename, strerror(errno));
    }
    else
    {
        // We don't echo time and date again because outBasic above just echoed them.
        outErrorSilent("=================[%s]=================", current_date_time.c_str());
    }
}

void oLog::Close()
{
    if(m_normalFile != NULL)
    {
        fflush(m_normalFile);
        fclose(m_normalFile);
        m_normalFile = NULL;
    }

    if(m_errorFile != NULL)
    {
        fflush(m_errorFile);
        fclose(m_errorFile);
        m_errorFile = NULL;
    }
}

void oLog::SetFileLoggingLevel(int32 level)
{
    if (level < LOG_LEVEL_NORMAL)
        level = LOG_LEVEL_NORMAL;

    m_fileLogLevel = level;
}

void oLog::SetDebugFlags(uint32 flags)
{
    mDebugFlags = flags;
}

void SessionLogWriter::write(const char* format, ...)
{
    if(!m_file)
        return;

    char out[32768];
    va_list ap;

    va_start(ap, format);

    std::string current_time = "[" + Util::GetCurrentDateTimeString() + "] ";
    sprintf(out, current_time.c_str());

    size_t l = strlen(out);
    vsnprintf(&out[l], 32768 - l, format, ap);
    fprintf(m_file, "%s\n", out);
    va_end(ap);
}

WorldLog::WorldLog()
{
    bEnabled = false;
    m_file = NULL;

    if(Config.MainConfig.GetBoolDefault("LogLevel", "World", false))
    {
        Log.Notice("WorldLog", "Enabling packetlog output to \"world.log\"");
        Enable();
    }
    else
    {
        Disable();
    }
}

void WorldLog::Enable()
{
    if(bEnabled)
        return;

    bEnabled = true;
    if(m_file != NULL)
    {
        Disable();
        bEnabled = true;
    }
    m_file = fopen("world.log", "a");
}

void WorldLog::Disable()
{
    if(!bEnabled)
        return;

    bEnabled = false;
    if(!m_file)
        return;

    fflush(m_file);
    fclose(m_file);
    m_file = NULL;
}

WorldLog::~WorldLog()
{
    if(m_file)
    {
        fclose(m_file);
        m_file = NULL;
    }
}

void SessionLogWriter::Open()
{
    m_file = fopen(m_filename, "a");
}

void SessionLogWriter::Close()
{
    if(!m_file) return;
    fflush(m_file);
    fclose(m_file);
    m_file = NULL;
}

SessionLogWriter::SessionLogWriter(const char* filename, bool open)
{
    m_filename = strdup(filename);
    m_file = NULL;
    if(open)
        Open();
}

SessionLogWriter::~SessionLogWriter()
{
    if(m_file)
        Close();

    free(m_filename);
}