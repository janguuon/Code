#pragma once
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/SAX.h>
#include <libxml/SAX2.h>

class CInfo;
class CSAX_Handler
{
public:
    CSAX_Handler();
    ~CSAX_Handler();

	static void OnStartElementsNs(void* ctx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI,
        int nb_namespaces,
        const xmlChar** namespaces,
        int nb_attributes,
        int nb_defaulted,
        const xmlChar** attributes);

	static void OnEndElementsNs(void* ctx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI);

	static void OnCharacters(void* ctx, const xmlChar* ch, int len);

private:
    static CInfo* info;
};

