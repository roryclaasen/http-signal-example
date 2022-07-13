#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

void Post(std::string host, std::string path, std::string data)
{
    try {
        std::cout << "Post to " << path << "\n";

        httplib::Client client(host);
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        client.enable_server_certificate_verification(false);
#endif

        auto response = client.Post(path.c_str(), data, "application/json");
        if (response.error() == httplib::Error::Success)
        {
            std::cout << "Post Response " << response->body << "\n";
        }
        else
        {
            std::cout << "Failed to send post to " << path << "\n";
        }
    }
    catch (...)
    {
        std::cout << "Failed to send post to " << path << "\n";
    }
}

std::string MakeMessage(std::string message)
{
    std::string title = "TestApplication";
    return "{ \"title\": \"" + title + "\", \"message\": \"" + message + "\" }";
}

int main()
{
    const auto httpHost = "http://localhost:5175";

    Post(httpHost, "/data", MakeMessage("This is a blocking call"));

    std::thread start_thread([httpHost] { Post(httpHost, "/start", MakeMessage("This is the start of the program")); });
    std::thread crash_thread([httpHost] { Post(httpHost, "/crash", MakeMessage("Oh no you crashed")); });

    start_thread.join();
    crash_thread.join();

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    const auto httpsHost = "https://localhost:7286";

    Post(httpsHost, "/data", MakeMessage("This came from https"));
#endif
}
