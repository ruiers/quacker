class Logger
{
    enum
    {
        LOCAL_NONE = 0 << 0,
        LOCAL_FILE = 1 << 0,
        ETHER_NTCP = 1 << 1,
        ETHER_NUDP = 1 << 2
    };

    struct log_device
    {
        FILE* log_steam;
        int   log_fd;
        char  log_type;
        void* netClient;
    } ;

private:
    static const int max_dev_num = 10;
    struct log_device* log_devs[max_dev_num];
    int    num_log_device;

public:
    Logger()
    {
        num_log_device = 0;
    };

    void   add_time();
    void Hexdump(const char* data, int len);
    void toFile(const char* name);
    int  to_devices(const char *format, ...);
};

extern Logger Log;

#define LogFun(...) { Log.to_devices("%s ", __func__);}
#define LogFLi(...) { Log.to_devices("%s %d ", __FILE__, __LINE__);}
#define LogErr(...) { LogFLi(); Log.to_devices(__VA_ARGS__); Log.to_devices("\n"); }
#define LogVer(...) { Log.to_devices(__VA_ARGS__); Log.to_devices("\n"); }
#define LogDbg(...) { Log.add_time(); Log.to_devices(__VA_ARGS__); Log.to_devices("\n"); }
#define LogHex(...) { Log.Hexdump(__VA_ARGS__); }
