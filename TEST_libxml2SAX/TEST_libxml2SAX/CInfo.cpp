#include "CInfo.h"
#include "CCar.h"
#include "CDriver.h"

void CInfo::Info_Initialize()
{
    Car_Start = &CCar::OnStartElememnt;
    Car_End = &CCar::OnEndElement;
    Car_Char = &CCar::OnCharacter;

    Driver_Start = &CDriver::OnStartElememnt;
    Driver_End = &CDriver::OnEndElement;
    Driver_Char = &CDriver::OnCharacter;
}

void CInfo::OnStartElememnt(
    void* ctx,
    const xmlChar* localname,
    const xmlChar* prefix,
    const xmlChar* URI,
    int nb_namespaces,
    const xmlChar** namespaces,
    int nb_attributes,
    int nb_defaulted,
    const xmlChar** attributes)
{
    if (!strcmp("car", (const char*)localname)) pcar = new CCar;
    else if (!strcmp("driver", (const char*)localname))  pdriver = new CDriver;

    if (nullptr != pcar)
    {
        (pcar->*Car_Start)(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
    }
    else if (nullptr != pdriver)
    {
        (pdriver->*Driver_Start)(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
    }

}

bool CInfo::OnEndElement(void* ctx,
    const xmlChar* localname,
    const xmlChar* prefix,
    const xmlChar* URI)
{
    if (nullptr != pcar)
    {
        if ((pcar->*Car_End)(ctx, localname, prefix, URI))
        {
            vElement.push_back(new CCar(pcar));
            delete pcar;
            pcar = nullptr;
        }
    }
    else if (nullptr != pdriver)
    {
        if ((pdriver->*Driver_End)(ctx, localname, prefix, URI))
        {
            vElement.push_back(new CDriver(pdriver));
            delete pdriver;
            pdriver = nullptr;
        }
    }

    return false;
}

void CInfo::OnCharacter(void* ctx, const xmlChar* ch, int len)
{
     if (nullptr != pcar)
        (pcar->*Car_Char)(ctx, ch, len);
    else if (nullptr != pdriver)
        (pdriver->*Driver_Char)(ctx, ch, len);
}

void CInfo::print()
{
    int size = vElement.size();

    for (int i = 0; i < size; ++i)
    {
        vElement[i]->print();

        CInfo* tmp = vElement[i];
        delete tmp;
    }
}
