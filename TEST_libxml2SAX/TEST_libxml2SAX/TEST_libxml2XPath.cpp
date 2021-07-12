#include <iostream>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

using namespace std;

static void print_node(xmlNodeSetPtr nodes)
{
	xmlNodePtr cur;
	int size;
	char buff[1024] = { 0 };

	size = (nodes) ? nodes->nodeNr : 0;
	std::cout << size << " nodes\n";


	for (int i = 0; i < size; ++i)
	{
		if (nodes->nodeTab[i]->type == XML_NAMESPACE_DECL)
		{
			xmlNsPtr ns;

			ns = (xmlNsPtr)nodes->nodeTab[i];
			cur = (xmlNodePtr)ns->next;

			if (cur->ns)
			{
				std::cout << "namespace\"" << ns->prefix << "\" = " << "\"" << ns->href << "\" for node " << ns->href << " : " << cur->name << "\"";
			}
			else
			{
				printf("namespace \"%s\" = \"%s\" for node %s ", ns->prefix, ns->href, cur->name);
			}
		}
		else if (nodes->nodeTab[i]->type == XML_ELEMENT_NODE)
		{
			cur = nodes->nodeTab[i];
			if (cur->ns) 
			{
				printf("element node \"%s : %s \"" , cur->ns->href, cur->name);
			}
			else 
			{
				printf("element node \"%s\"\n" ,cur->name);
			}
		}
		else 
		{
			cur = nodes->nodeTab[i];
			printf("node \"%s \" : type %d " , cur-> name, cur-> type);
		}
	}
}

int StartXpath(xmlDocPtr doc, xmlChar* xpath_expr)
{
	xmlXPathContextPtr xpath_ctx;
	xmlXPathObjectPtr xpath_obj;

	xpath_ctx = xmlXPathNewContext(doc);

	if (NULL == xpath_ctx)
	{
		xmlFree(doc);
		return 0;
	}

	xpath_obj = xmlXPathEvalExpression(xpath_expr, xpath_ctx);

	if (NULL == xpath_obj)
	{
		xmlXPathFreeContext(xpath_ctx);
		xmlFreeDoc(doc);
		return 0;
	}

	std::cout << xpath_expr << endl;
	print_node(xpath_obj->nodesetval);

	xmlXPathFreeObject(xpath_obj);
	xmlXPathFreeContext(xpath_ctx);

	return 0;
}

int main()
{
	xmlDocPtr doc;
	xmlNodePtr root_elemental = NULL;
	int ret = 0;


	LIBXML_TEST_VERSION;

	doc = xmlReadFile("C:/Users/ojw/Desktop/slide2.xml", NULL, 0);
	if (doc == NULL)
	{
		return 0;
	}

	StartXpath(doc, (xmlChar*)"//bar");
	StartXpath(doc, (xmlChar*)"/cars/car");

	xmlFree(doc);
	xmlCleanupParser();

	return 0;
}