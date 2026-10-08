/* NetBSD: stubs for pruned DCN32+/DML2/HDCP symbols still referenced by DC. */
#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD$");

#include "dc.h"
#include "dml2/dml2_wrapper.h"
#include "hdcp_msg_types.h"

bool dml2_create(const struct dc *in_dc,
		 const struct dml2_configuration_options *config,
		 struct dml2_context **dml2)
{
	if (dml2)
		*dml2 = NULL;
	return false;
}

bool dml2_create_copy(struct dml2_context **dst_dml2,
		      struct dml2_context *src_dml2)
{
	if (dst_dml2)
		*dst_dml2 = NULL;
	return false;
}

void dml2_copy(struct dml2_context *dst_dml2, struct dml2_context *src_dml2)
{
}

void dml2_destroy(struct dml2_context *dml2)
{
}

enum hdcp_message_status
dc_process_hdcp_msg(enum signal_type signal, struct dc_link *link,
		    struct hdcp_protection_message *message_info)
{
	return HDCP_MESSAGE_UNSUPPORTED;
}
