#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include <json/json.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}
double convertKelvinToCelsius(const double &temperatureInKelvin) {
    return temperatureInKelvin - 273.15;
}
int getIdOfCity(const std::string &nameOfCity) {
    const std::string pathToFileWithAllCities = "city.list.json";
    std::ifstream file(pathToFileWithAllCities);
    Json::Value root;
    file >> root;    

    int resultId{0};
    for (int i = 0; i < root.size(); i++) {
        Json::Value city = root[i];
        if(nameOfCity == city["name"].asString()) {
            resultId = city["id"].asInt();
            break;
        }
    }
    if (!resultId) {
        std::cout << "We didn't find any city with this name\n";
        return 0;
    }
    return resultId;
}
Json::Value getWeatherData(int idOfCity, const std::string &readBuffer) {
    CURL *curl;
    CURLcode res;
    Json::Value jsonData;
    Json::Reader jsonReader;

    curl = curl_easy_init();
    if (curl) {
        const char *appikey = "769846b0b947bd962f1c2f8f261ef889";
        std::string id = std::to_string(idOfCity);
        const char *id_cstr = id.c_str();
        std::string path = "https://api.openweathermap.org/data/2.5/weather?id=" + id + "&appid=" + appikey;
        const char *path_cstr = path.c_str();

        curl_easy_setopt(curl, CURLOPT_URL, path_cstr);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (jsonReader.parse(readBuffer, jsonData)) {
            return jsonData;
        } else {
            std::cout << "Error parsing JSON data" << std::endl;
            return Json::Value();
        }
    }
    return Json::Value();
}
bool parseJsonData(const std::string &readBuffer, Json::Value &jsonData) {
    Json::Reader jsonReader;
    std::ofstream jsonFile;
    const std::string pathToJsonFile = "weather.json";
    jsonFile.open(pathToJsonFile);
    if (!jsonReader.parse(readBuffer, jsonData)) {
        std::cout << "Error parsing JSON data" << std::endl;
        return false;
    } else {
        jsonFile << jsonData.toStyledString() << std::endl;
        return true;
    }
}
void printInfoAboutCity(const Json::Value &jsonData) {
    double tempCelsius = convertKelvinToCelsius(jsonData["main"]["temp"].asDouble());
    double tempFeelsLikeInCelsius = convertKelvinToCelsius(jsonData["main"]["feels_like"].asDouble());
    std::string city = jsonData["name"].asString();
    std::cout << "Temperature in " << city << " in Celsius: " << tempCelsius << '\n'
              << "Feels like " << tempFeelsLikeInCelsius << '\n';
}
int main() {
    std::string nameOfCity;
    std::cout << "Enter name of city:";
    std::cin >> nameOfCity;
    int idOfCity = getIdOfCity(nameOfCity);
    if (!idOfCity) {
        return 0; 
    } 

    std::string readBuffer;
    if (!getWeatherData(idOfCity, readBuffer)) {
        return 1; // return 1 if API request fails
    }

    Json::Value jsonData;
    if(!parseJsonData(readBuffer, jsonData)) {
        return 2;
    }

    printInfoAboutCity(jsonData);
    return 0;
}
