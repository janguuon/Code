#pragma once
#include <unordered_map>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/SAX.h>
#include <libxml/SAX2.h>
#include "CInfo.h"


class CCar : public CInfo
{
public:
    CCar();
    CCar(CCar* car)
    {
        for (auto iter = car->m_mapElement.begin(); iter != car->m_mapElement.end(); iter++)
        {
            m_mapElement.insert(make_pair(iter->first, std::pair<Attribute, std::string>(iter->second.first,iter->second.second)));
        }
    }
    ~CCar();

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
};

