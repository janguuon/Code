#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/SAX.h>
#include <libxml/SAX2.h>


struct Attribute
{
public:
    xmlChar* m_Attr;
    xmlChar m_AttrValue[256];
};

class CDriver;
class CCar;
class CInfo
{
public:

    CInfo() 
    {
        Info_Initialize();
    }

    virtual ~CInfo() {}
    
private:
    void Info_Initialize();

public:
    virtual void OnStartElememnt(
        void* ctx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI,
        int nb_namespaces,
        const xmlChar** namespaces,
        int nb_attributes,
        int nb_defaulted,
        const xmlChar** attributes);

    virtual bool OnEndElement(void* ctx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI);

    virtual void OnCharacter(void* ctx, const xmlChar* ch, int len);

    virtual void print();

private:
    void (CCar::* Car_Start)(void* ctx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI,
        int nb_namespaces,
        const xmlChar** namespaces,
        int nb_attributes,
        int nb_defaulted,
        const xmlChar** attributes);

    bool (CCar::* Car_End)(void* ctx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI);

    void (CCar::* Car_Char)(void* ctx, const xmlChar* ch, int len);

    void (CDriver::* Driver_Start)(void* ctx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI,
        int nb_namespaces,
        const xmlChar** namespaces,
        int nb_attributes,
        int nb_defaulted,
        const xmlChar** attributes);

    bool (CDriver::* Driver_End)(void* ctx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI);

    void (CDriver::* Driver_Char)(void* ctx, const xmlChar* ch, int len);

   

protected:
    std::unordered_map<xmlChar*, std::pair<Attribute, std::string>> m_mapElement;
    xmlChar* m_elemname;
    Attribute m_attr;
    std::string m_text;

private:
    CCar* pcar = nullptr;
    CDriver* pdriver = nullptr;
    std::vector<CInfo*> vElement;
};
