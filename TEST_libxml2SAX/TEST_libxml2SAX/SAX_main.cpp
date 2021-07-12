#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <vector>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/SAX.h>
#include <libxml/SAX2.h>

#include "CSAX_Handler.h"

#define MY_MAX 1024

int read_xmlFile(FILE* file)
{
    char str[MY_MAX] = { 0 };
    int res = static_cast<int>(fread(str, static_cast<size_t>(1), static_cast<size_t>(4), file));

    if (res <= 0) return 1;

    xmlSAXHandler SAXHandler;
    memset(&SAXHandler, 0, sizeof(xmlSAXHandler));

    // main
    CSAX_Handler * handler = new CSAX_Handler;

    SAXHandler.initialized = XML_SAX2_MAGIC;
    SAXHandler.startElementNs = handler->OnStartElementsNs;
    SAXHandler.characters = handler->OnCharacters;
    SAXHandler.endElementNs = handler->OnEndElementsNs;

    xmlParserCtxtPtr ctxt = xmlCreatePushParserCtxt(&SAXHandler, NULL, str, res, NULL);

    while ((res = static_cast<int>(fread(str, static_cast<size_t>(1), sizeof(str), file))) > 0)
    {
        if (xmlParseChunk(ctxt, str, res, 0))
        {
            xmlParserError(ctxt, "xmlParseChunk");

            return 1;
        }
    }

    delete handler;
    xmlParseChunk(ctxt, str, res, 1);
    xmlFreeParserCtxt(ctxt);
    xmlCleanupParser();
    xmlMemoryDump();
    return 0;
}

int main()
{
    std::locale::global(std::locale(".UTF-8"));
    FILE* file = fopen("../slide2.xml", "rb");
    
    if (!file)
    {
        std::cout << "file open error" << '\n';
        return 0;
    }
    if (read_xmlFile(file))
    {
        std::cout << "xml read error" << '\n';
        return 0;
    }

    fclose(file);
    return 0;
}
