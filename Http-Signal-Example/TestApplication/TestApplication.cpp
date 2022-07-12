#include <httplib.h>
#include <iostream>

int main()
{
    httplib::Client client("http://localhost:5175");

    std::string path = "/crash";
    std::string postBody = "{ \"title\": \"TestApplication\", \"message\": \"This is an example\" }";

    auto response = client.Post(path.c_str(), postBody, "application/json");

    std::cout << "Response Status: " << response->status << "\n";
    std::cout << "Response Body: " << response->body;
}