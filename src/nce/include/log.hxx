#pragma once

#define LOGWARN(msg) log_warning((msg), __FILE__, __func__, __LINE__)
#define LOGERROR(msg) log_error((msg), __FILE__, __func__, __LINE__)
#define LOGINFO(msg) log_info((msg))
#define LOGHEADER(msg) log_info((msg))
