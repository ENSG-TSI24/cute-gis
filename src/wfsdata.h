#ifndef WFSDATA_H
#define WFSDATA_H

#include <string>

class WFSData {
public:
    WFSData(const std::string& url);
    bool fetchData();
    const std::string& getResponse() const { return response; }

private:
    std::string wfs_url;
    std::string response;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
};

#endif // WFSDATA_H
