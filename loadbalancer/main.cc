#include <drogon/drogon.h>

int main()
{
    drogon::app().loadConfigFile("/drogon/app/config.json");
    drogon::app().run();
    return 0;
}
