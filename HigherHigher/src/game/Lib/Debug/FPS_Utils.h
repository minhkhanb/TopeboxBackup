#if !_FPS_UTILS_H
#define FPS_MAX_SLOT (20)
#include <time.h>
class FPS_Utils
{
public:
	char _log_info[10240];
	bool _log_used[FPS_MAX_SLOT];
	time_t _log_time[FPS_MAX_SLOT];
	void Begin(int log_index);
	void End(int log_index);
	void ClearAllLog();
	const char* GetLogInfo();
};
#endif