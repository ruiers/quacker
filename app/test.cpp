#include <stdio.h>
#include "../inc/linux/logger.h"

int main(int argc, char const *argv[])
{
    double version = 1.0;

    LogVer("Version %f", version);
    LogDbg("Debug info %f", version);
    LogErr("Error info %f", version);
    LogHex("done", 4);

    return 0;
}
