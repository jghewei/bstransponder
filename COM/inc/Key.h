#ifndef _KEY_H_
#define _KEY_H_

#include <stdio.h>
#include <string.h>
#include <string>

namespace KeyType
{
    enum Enum
    {
        First,
        System,
        Shelf,
        Slot,
        Card,
        Port,
        Fan,
        Psu,
        Osc,
        Mgmt,
        Eth,
        Otu,
        Odu,
        Och,
        Fc,
        Oc,
        Stm,
        Snmp,
        User,
        Last,
    };
}

static const char *KeyName[] = 
{
    "first",
    "System",
    "shelf",
    "slot",
    "card",
    "port",
    "fan",
    "psu",
    "osc",
    "mgmt",
    "eth",
    "otu",
    "odu",
    "och",
    "fc",
    "oc",
    "stm",
    "Snmp"
    "last",
};
    
class Key
{
public:
    Key()
    {
        type_ = KeyType::First;
        id_ = 0;
    }

    Key(KeyType::Enum type, int id)
    {
        type_ = type;
        id_ = id;
    }
    
    Key(std::string &name)
    {
        char type[16] = {0};
        int id = 0;
        sscanf(name.c_str(), "%s-%d", type, &id);
        type_ = KeyType::First;
        id_ = 0;
        for (int t = (int)KeyType::First; t < (int)KeyType::Last; t++)
        {
            if (strcmp(type, KeyName[t]) == 0)
            {
                type_ = (KeyType::Enum)t;
                id_ = id;
                break;
            }
        }
    }

    bool operator == (const Key& key)
    {
        if (this->type_ == key.type() && this->id_ == key.id())
            return true;
        else
            return false;
    }

    Key& operator = (const Key& key)
    {
        this->type_ = key.type();
        this->id_ = key.id();
        return *this;
    }

    KeyType::Enum type() const { return type_; }
    
    int id() const { return id_; }
    
    std::string name()
    {
        char name[32] = {0};
        sprintf(name, "%s-%d", KeyName[(int)type_], id_);
        return name;
    }
    
    void set_type(KeyType::Enum type) { type_ = type; }
    
    void set_id(int id) { id_ = id; }
    
    void set_key(KeyType::Enum type, int id) { type_ = type; id_ = id; }
    
private:
    KeyType::Enum type_;
    int id_;
};

namespace LedType
{
    enum Enum
    {
        PWR,
        MAJ,
        MIN,
        LNK,
        ACT,
        TX,
        RX,
        ALM,
    };
}

namespace LedState
{
    enum Enum
    {
        OFF,
        ON,
        Blink,
    };
}

namespace LedColor
{
    enum Enum
    {
        OFF,
        RED,
        GREEN,
        AMBER,
    };
}

#endif

