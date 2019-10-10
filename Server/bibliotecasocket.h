#ifdef __MINGW32__
    #define _WIN32_WINNT 0x501
    #include <ws2tcpip.h>
#endif
#ifdef __linux__
#endif