#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <vector>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/SAX.h>
#include <libxml/SAX2.h>

#define MY_MAX 1024

static void OnStartElementNs(
void *ctx,
const xmlChar *localname,
const xmlChar * prefix,
const xmlChar * URI,
int nb_namespaces,
const xmlChar ** namespaces,
int nb_attributes,
int nb_defaulted,
const xmlChar **attributes)
{
    if (0 < nb_attributes)
    {
        const int fields = 5;    /*(localname/prefix/URI/value/end)*/
        xmlChar* value = nullptr;
        size_t size;
        int length;
        std::vector<std::pair<const xmlChar*, xmlChar*>> vAttr;
        for (int i = 0; i < nb_attributes; i++) 
        {
            const xmlChar* localname = attributes[i * fields + 0];
            const xmlChar* prefix = attributes[i * fields + 1];
            const xmlChar* URI = attributes[i * fields + 2];
            const xmlChar* value_start = attributes[i * fields + 3];
            const xmlChar* value_end = attributes[i * fields + 4];

            size = value_end - value_start;
            value = (xmlChar*)malloc(sizeof(xmlChar) * size + 3);
            memcpy(value, value_start, size);
            value[size] = '\0';

            length = static_cast<int>(strlen((const char*)value));
            for (int j = 0; j < length + 1; ++j)
            {
                value[(length - j) + 1] = value[length - j];
            }
            value[0] = '\"';
            value[length + 1] = '\"';
            value[length + 2] = '\0';

            vAttr.emplace_back(std::pair<const xmlChar*, xmlChar*>(localname, value));
        }
        printf("<%s ", localname);
        for (int i = 0; i < nb_attributes; ++i)
        {
            printf("%s = %s", vAttr[i].first, vAttr[i].second);
        }
        printf(">\n");

        free(value);
    }
    else
    {
       printf("<%s>\n", localname);
    }
}
static void OnEndElementNs(void * ctx,
    const xmlChar* localname,
    const xmlChar * prefix,
    const xmlChar* URI)
{
    printf("</%s>\n", localname);
}

static void OnCharacters(void* ctx, const xmlChar* ch, int len)
{
    char str[MY_MAX] = {0};
    strncpy(str,(const char*)ch, len);
    str[len] = '\0';
    std::string tmp = str;

    tmp.erase(remove(tmp.begin(), tmp.end(), '\n'), tmp.end());

    if (len > 3)
    {
        std::cout << tmp << '\n';
    }
}

xmlSAXHandler make_SAX_Handler()
{
    xmlSAXHandler SAXHandler;
    memset(&SAXHandler, 0, sizeof(xmlSAXHandler));

    SAXHandler.initialized = XML_SAX2_MAGIC;
    SAXHandler.startElementNs = OnStartElementNs;
    SAXHandler.characters = OnCharacters;
    SAXHandler.endElementNs = OnEndElementNs;
    
    return SAXHandler;
}

int read_xmlFile(FILE* file)
{
    char str[MY_MAX] = {0};
    int res = static_cast<int>(fread(str, static_cast<size_t>(1), static_cast<size_t>(4), file));

    if (res <= 0) return 1;

    xmlSAXHandler SAXHandler = make_SAX_Handler();

    xmlParserCtxtPtr ctxt = xmlCreatePushParserCtxt(&SAXHandler, NULL, str, res, NULL);

    while ((res = static_cast<int>(fread(str, static_cast<size_t>(1), sizeof(str), file))) > 0) 
    {
        if (xmlParseChunk(ctxt, str, res, 0))
        {
            xmlParserError(ctxt, "xmlParseChunk");

            return 1;
        }
    }
    
    xmlParseChunk(ctxt, str, 0, 1);
    xmlFreeParserCtxt(ctxt);
    xmlCleanupParser();
    xmlMemoryDump();
    return 0;
}

int main()
{
    std::locale::global(std::locale(".UTF-8"));
    FILE* file = fopen("C:/Users/ojw/Desktop/slide2.xml", "rb");
    
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
