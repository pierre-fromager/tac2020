
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef LOGGER_H
#define LOGGER_H

#include "color.h"

    typedef enum
    {
        LOG_NONE,
        LOG_ERROR,
        LOG_WARN,
        LOG_INFO,
        LOG_DEBUG,
        LOG_VERBOSE
    } log_level_t;

#define __IC_INFO "\xE2\x84\xB9"
#define __IC_CORRECTBOX "\xE2\x98\x91"
#define __IC_WRONGBOX "\xE2\x9D\x8E"
#define __IC_WRONG_ANSWER "\xE2\x9C\x96"
#define __IC_CORRECT_ANSWER "\xE2\x9C\x94"
#define __IC__WARNING_ALERT "\xE2\x9A\xA0"
#define __IC_LOADING_SIGN "\xE2\x8C\x9B"
#define __IC_RECYCLE_SIGN "\xE2\x99\xBB"
#define __IC_FACE_RELAX "\xE2\x98\xBA"
#define __IC_FACE_SLEEPING "\xF0\x9F\x98\xB4"
#define __IC_FACE_SKULL "\xE2\x98\xA0"
#ifndef LOGGER_LOG_LEVEL
#define LOGGER_LOG_LEVEL LOG_ERROR
#endif

#define LOGGER_E_FMT " Error [%d] : %s\n"

#define _log_(color, format) \
    printf(color format RESET);

#define _log_args_(color, format, args...) \
    printf(color format RESET, ##args);

#define log_e(ec, msg)                          \
    if (0 != ec)                                \
    {                                           \
        _log_args_(RED, LOGGER_E_FMT, ec, msg); \
        exit(EXIT_FAILURE);                     \
    }

#define log_error(format, args...)       \
    if (LOGGER_LOG_LEVEL >= LOG_ERROR)   \
    {                                    \
        _log_args_(RED, format, ##args); \
    }

#define log_warn(format, args...)        \
    if (LOGGER_LOG_LEVEL >= LOG_WARN)    \
    {                                    \
        _log_args_(YEL, format, ##args); \
    }

#define log_info(format, args...)        \
    if (LOGGER_LOG_LEVEL >= LOG_INFO)    \
    {                                    \
        _log_args_(WHT, format, ##args); \
    }

#define log_debug(format, args...)       \
    if (LOGGER_LOG_LEVEL >= LOG_DEBUG)   \
    {                                    \
        _log_args_(GRN, format, ##args); \
    }

#define log_verbose(format, args...)     \
    if (LOGGER_LOG_LEVEL >= LOG_VERBOSE) \
    {                                    \
        _log_args_(GRN, format, ##args); \
    }

#endif // ifndef LOGGER_H

#ifdef __cplusplus
}
#endif