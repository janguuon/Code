#include <iostream>
#include <libxml/tree.h>
#include <libxml/parser.h>


static void print_Attributes(xmlNode* a_node)
{
    _xmlAttr * current_Attribute = a_node->properties;
    while (nullptr != current_Attribute)
    {
        printf("attr [%s] : [%s]\n", current_Attribute->name, current_Attribute->children->content);

        current_Attribute = current_Attribute-> next;
    }
}

static void print_Element_names(xmlNode* a_node)
{
    xmlNode* current_Node = NULL;

    for (current_Node = a_node; current_Node; current_Node = current_Node->next) 
    {
        if (current_Node->type == XML_ELEMENT_NODE) 
        {
            printf("element [%s]\n", current_Node->name);

            if(nullptr != current_Node->properties) print_Attributes(current_Node);
        }
        else if (current_Node->type == XML_TEXT_NODE)
        {
            const char ch = current_Node->content[0];
            if ('\n' != ch)
            {
                printf("text [% s]\n", current_Node->content);
            }
        }
        print_Element_names(current_Node->children);
    }
}

int main()
{
    xmlDoc* doc = NULL;
    xmlNode* root_element = NULL;

    doc = xmlReadFile("C:/Users/ojw/Desktop/slide2.xml", NULL, 0);

    if (NULL == doc)
    {
        return 0;
    }

    root_element = xmlDocGetRootElement(doc);
    print_Element_names(root_element);

    xmlFreeDoc(doc);
    xmlCleanupParser();
	return 0;
}
