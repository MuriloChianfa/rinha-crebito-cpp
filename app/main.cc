#include <drogon/HttpAppFramework.h>

using namespace drogon;

int main()
{
    const char* API_PORT = std::getenv("API_PORT") ? std::getenv("API_PORT") : "8848";

    drogon::app().loadConfigFile("/drogon/app/config.json");
    drogon::app().addListener("0.0.0.0", atoi(API_PORT));
    drogon::app().run();

    return 0;
}
