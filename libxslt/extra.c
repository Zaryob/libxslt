/*
 * extra.c: Implementation of non-standard features
 *
 * Reference:
 *   Michael Kay "XSLT Programmer's Reference" pp 637-643
 *   The node-set() extension function
 *
 * See Copyright for the status of this software.
 *
 * Daniel.Veillard@imag.fr
 */

#include "xsltconfig.h"

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/hash.h>
#include <libxml/xmlerror.h>
#include <libxml/parserInternals.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "extensions.h"
#include "variables.h"
#include "transform.h"
#include "extra.h"

#define DEBUG_EXTRA

/************************************************************************
 * 									*
 * 		Handling of XSLT debugging				*
 * 									*
 ************************************************************************/

/**
 * xsltDebug:
 * @ctxt:  an XSLT processing context
 * @node:  The current node
 * @inst:  the instruction in the stylesheet
 * @comp:  precomputed informations
 *
 * Process an debug node
 */
void 
xsltDebug(xsltTransformContextPtr ctxt, xmlNodePtr node, xmlNodePtr inst,
	  xsltStylePreCompPtr comp) {
    int i, j;

    fprintf(stdout, "Templates:\n");
    for (i = 0, j = ctxt->templNr - 1;((i < 5) && (j >= 0));i++,j--) {
	fprintf(stdout, "#%d ", i);
	if (ctxt->templTab[j]->name != NULL)
	    fprintf(stdout, "name %s ", ctxt->templTab[j]->name);
	if (ctxt->templTab[j]->match != NULL)
	    fprintf(stdout, "name %s ", ctxt->templTab[j]->match);
	if (ctxt->templTab[j]->mode != NULL)
	    fprintf(stdout, "name %s ", ctxt->templTab[j]->mode);
	fprintf(stdout, "\n");
    }
    fprintf(stdout, "Variables:\n");
    for (i = 0, j = ctxt->varsNr - 1;((i < 10) && (j >= 0));i++,j--) {
	xsltStackElemPtr cur;

	if (ctxt->varsTab[j] == NULL)
	    continue;
	fprintf(stdout, "#%d\n", i);
	cur = ctxt->varsTab[j];
	while (cur != NULL) {
	    if (cur->type == XSLT_ELEM_VARIABLE)
		fprintf(stdout, "var ");
	    else if (cur->type == XSLT_ELEM_PARAM)
		fprintf(stdout, "param ");
	    else {
		fprintf(stdout, "corrupted !!!\n");
	    }
	    if (cur->name != NULL)
		fprintf(stdout, "%s ", cur->name);
	    else
		fprintf(stdout, "noname !!!!");
	    if (cur->value != NULL) {
		xmlXPathDebugDumpObject(stdout, cur->value, 1);
	    } else {
		fprintf(stdout, "NULL !!!!");
	    }
	    fprintf(stdout, "\n");
	    cur = cur->next;
	}
	
    }
}

/************************************************************************
 * 									*
 * 		Classic extensions as described by M. Kay		*
 * 									*
 ************************************************************************/

/**
 * xsltFunctionNodeSet:
 * @ctxt:  the XPath Parser context
 * @nargs:  the number of arguments
 *
 * Implement the node-set() XSLT function
 *   node-set node-set(result-tree)
 *
 * This function is available in libxslt, saxon or xt namespace.
 */
void
xsltFunctionNodeSet(xmlXPathParserContextPtr ctxt, int nargs){
    if (nargs != 1) {
        xsltGenericError(xsltGenericErrorContext,
		"node-set() : expects one result-tree arg\n");
	ctxt->error = XPATH_INVALID_ARITY;
	return;
    }
    if ((ctxt->value == NULL) || (ctxt->value->type != XPATH_XSLT_TREE)) {
	xsltGenericError(xsltGenericErrorContext,
	    "node-set() invalid arg expecting a result tree\n");
	ctxt->error = XPATH_INVALID_TYPE;
	return;
    }
    ctxt->value->type = XPATH_NODESET;
    ctxt->value->boolval = 1;
}

/**
 * xsltRegisterExtras:
 * @ctxt:  a XSLT process context
 *
 * Registers the built-in extensions
 */
void
xsltRegisterExtras(xsltTransformContextPtr ctxt) {
    xsltRegisterExtFunction(ctxt, (const xmlChar *) "node-set",
	                    XSLT_LIBXSLT_NAMESPACE, xsltFunctionNodeSet);
    xsltRegisterExtFunction(ctxt, (const xmlChar *) "node-set",
	                    XSLT_SAXON_NAMESPACE, xsltFunctionNodeSet);
    xsltRegisterExtFunction(ctxt, (const xmlChar *) "node-set",
	                    XSLT_XT_NAMESPACE, xsltFunctionNodeSet);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "debug",
	                    XSLT_LIBXSLT_NAMESPACE, xsltDebug);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "output",
	                    XSLT_SAXON_NAMESPACE, xsltDocumentElem);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "write",
	                    XSLT_SAXON_NAMESPACE, xsltDocumentElem);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "document",
	                    XSLT_XT_NAMESPACE, xsltDocumentElem);
}