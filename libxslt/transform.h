/*
 * transform.h: Interfaces, constants and types related to the XSLT engine
 *            transform part.
 *
 * See Copyright for the status of this software.
 *
 * Daniel.Veillard@imag.fr
 */

#ifndef __XML_XSLT_TRANSFORM_H__
#define __XML_XSLT_TRANSFORM_H__

#include <libxml/tree.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Interfaces
 */
xmlDocPtr	xsltApplyStylesheet	(xsltStylesheetPtr style,
					 xmlDocPtr doc);
void		xsltApplyOneTemplate	(xsltTransformContextPtr ctxt,
					 xmlNodePtr node,
					 xmlNodePtr list);
#ifdef __cplusplus
}
#endif

#endif /* __XML_XSLT_TRANSFORM_H__ */
