#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include <json/json.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?lat=50.4&lon=30.5&appid=769846b0b947bd962f1c2f8f261ef889");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        Json::Value jsonData;
        Json::Reader jsonReader;

        std::ofstream out_file;
        out_file.open("weather.json");

        if (jsonReader.parse(readBuffer, jsonData)) {
            out_file << jsonData.toStyledString() << std::endl;
        } else {
            std::cout << "Error parsing JSON data" << std::endl;
        }

        double temp_kelvin = jsonData["main"]["temp"].asDouble();
        std::string city = jsonData["name"].asString();
        double temp_celsius = temp_kelvin - 273.15;
        std::cout << "Temperature in " << city << "in Celsius: " << temp_celsius << std::endl;


        // std::cout << readBuffer << std::endl;
    }


    return 0;
}