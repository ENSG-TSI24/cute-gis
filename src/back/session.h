#ifndef SESSIONVISITOR_H
#define SESSIONVISITOR_H

#include <nlohmann/json.hpp>
#include <string>

class Session {
public:
    Session();
    ~Session();
    void updateFile();
    void addToJson(const char* path);   
    std::vector<std::string> getLayers();
    std::shared_ptr<nlohmann::json> m_json;


};
#endif