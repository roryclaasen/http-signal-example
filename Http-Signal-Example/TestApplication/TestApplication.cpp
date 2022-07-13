#include <iostream>

#include <httplib.h>

void Post(std::string path, std::string data)
{
    try {
        std::cout << "Post to " << path << "\n";

        httplib::Client client("http://localhost:5175");

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
    Post("/data", MakeMessage("This is a blocking call"));

    std::thread start_thread([] { Post("/start", MakeMessage("This is the start of the program")); });
    std::thread crash_thread([] { Post("/crash", MakeMessage("Oh no you crashed")); });

    start_thread.join();
    crash_thread.join();
}
