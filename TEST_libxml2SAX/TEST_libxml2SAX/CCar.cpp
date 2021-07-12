#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <vector>
#include "CCar.h"

#define MY_MAX 1024

CCar::CCar()
{
}
CCar::~CCar()
{
}

void CCar::OnStartElememnt( void* ctx,
    const xmlChar* localname,
    const xmlChar* prefix,
    const xmlChar* URI,
    int nb_namespaces,
    const xmlChar** namespaces,
    int nb_attributes,
    int nb_defaulted,
    const xmlChar** attributes)
{
    m_elemname = (xmlChar*)localname;

    if (0 < nb_attributes)
    {
        const int fields = 5;    /*(localname/prefix/URI/value/end)*/
        xmlChar* value = nullptr;
        size_t size;
        int length;

        for (int i = 0; i < nb_attributes; i++)
        {
            const xmlChar* _localname = attributes[i * fields + 0];
            const xmlChar* _prefix = attributes[i * fields + 1];
            const xmlChar* _URI = attributes[i * fields + 2];
            const xmlChar* _value_start = attributes[i * fields + 3];
            const xmlChar* _value_end = attributes[i * fields + 4];

            size = _value_end - _value_start;
            value = (xmlChar*)malloc(sizeof(xmlChar) * size + 3);
            memcpy(value, _value_start, size);
            value[size] = '\0';

            length = static_cast<int>(strlen((const char*)value));
            for (int j = 0; j < length + 1; ++j)
            {
                value[(length - j) + 1] = value[length - j];
            }
            value[0] = '\"';
            value[length + 1] = '\"';
            value[length + 2] = '\0';

            m_attr.m_Attr = (xmlChar*)_localname;
            strcpy((char*)m_attr.m_AttrValue, (const char*)value);
        }

        free(value);
    }
}

bool CCar::OnEndElement(void* ctx,
    const xmlChar* localname,
    const xmlChar* prefix,
    const xmlChar* URI)
{
    if (!strcmp("car", (const char*)localname))
        return true;

    return false;
}

void CCar::OnCharacter(void* ctx, const xmlChar* ch, int len)
{
    char str[MY_MAX] = { 0 };
    strncpy(str, (const char*)ch, len);
    str[len] = '\0';
    std::string tmp = str;

    tmp.erase(remove(tmp.begin(), tmp.end(), '\n'), tmp.end());

    if (len > 3)
    {
        m_text = tmp;
    }

    m_mapElement.insert(make_pair(m_elemname, std::pair<Attribute, std::string>(m_attr, m_text)));

    m_attr.m_Attr = (xmlChar*)"";
    strcpy((char*)m_attr.m_AttrValue, "");
}

void CCar::print()
{
    for (auto iter = m_mapElement.begin(); iter != m_mapElement.end(); iter++)
    {
        std::cout << iter->first << " ";
        std::cout << iter->second.first.m_Attr << " " << iter->second.first.m_AttrValue << '\n';
        std::cout << iter->second.second << '\n';
    }
}