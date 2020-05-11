#ifndef _WEB_H_
#define _WEB_H_

#include "DanOS.h"
#include "goahead.h"

int WebInit(void);
void WebExit(void);

bool isWebPostMethod(Webs *wp);
std::string WebFormResponse(bool success, const char *message);

class CJson
{
public:
    CJson()
    {
        mJson = "{\"aaData\":[";
    };

    void End()
    {
        mJson += "]}";

        size_t found = mJson.rfind(",]");
        if (found != std::string::npos)
        {
            mJson.replace(found, 2, "]");
        } 
    }

    void AddEntry() { mJson += "{"; }
    void EndEntry()
    {
        mJson += "},";
        
        size_t found = mJson.rfind(",}");
        if (found != std::string::npos)
        {
            mJson.replace(found, 2, "}");
        }
    }

    void AddValue(const char *key, const std::string &value)
    {
        mJson += "\"";
        mJson += key;
        mJson += "\":\"";
        mJson += value;
        mJson += "\",";
    }
    
    void AddValue(const char *key, const char *value)
    {
        mJson += "\"";
        mJson += key;
        mJson += "\":\"";
        mJson += value;
        mJson += "\",";
    }

    void AddValue(const char *key, const uint64_t value)
    {
        mJson += "\"";
        mJson += key;
        mJson += "\":\"";
        mJson += to_string(value);
        mJson += "\",";
    }

    void AddValue(const char *key, const bool value)
    {
        mJson += "\"";
        mJson += key;
        mJson += "\":";
        mJson += to_string(value);
        mJson += ",";
    }

    void AddValue(const char *key, const int value)
    {
        mJson += "\"";
        mJson += key;
        mJson += "\":";
        mJson += to_string(value);
        mJson += ",";
    }

    void AddValue(const char *key, const float value)
    {
        mJson += "\"";
        mJson += key;
        mJson += "\":";
        mJson += to_string(value);
        mJson += ",";
    }

    void InsertEntry(const char *name, const std::string &value)
    {
        AddEntry();
        AddValue("n", name);
        AddValue("v", value);
        EndEntry();
    }

    void InsertEntry(const char *name, const int value)
    {
        AddEntry();
        AddValue("n", name);
        AddValue("v", value);
        EndEntry();
    }

    const char* GetBuffer() { return mJson.c_str(); }
    size_t GetLength() { return mJson.size(); }

private:
    std::string mJson;
};

#endif

