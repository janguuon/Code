#include "CSAX_Handler.h"
#include "CInfo.h"

CInfo* CSAX_Handler::info = nullptr;

CSAX_Handler::CSAX_Handler()
{
    info = new CInfo;
}

CSAX_Handler::~CSAX_Handler()
{
    info->print();
    delete info;
}

void CSAX_Handler::OnStartElementsNs(void* ctx,
    const xmlChar* localname,
    const xmlChar* prefix,
    const xmlChar* URI,
    int nb_namespaces,
    const xmlChar** namespaces,
    int nb_attributes,
    int nb_defaulted,
    const xmlChar** attributes)
{
    if (nullptr == info) return;
    
    info->OnStartElememnt(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
}

void CSAX_Handler::OnEndElementsNs(void* ctx,
    const xmlChar* localname,
    const xmlChar* prefix,
    const xmlChar* URI)
{
    if (nullptr == info) return;

    info->OnEndElement(ctx, localname, prefix, URI);
}

void CSAX_Handler::OnCharacters(void* ctx, const xmlChar* ch, int len)
{
    if (nullptr == info) return;

    info->OnCharacter(ctx, ch, len);
}