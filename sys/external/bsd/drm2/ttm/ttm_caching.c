#include <sys/cdefs.h>

#include <sys/param.h>

#include <uvm/uvm_extern.h>

#include <drm/ttm/ttm_caching.h>

pgprot_t
ttm_prot_from_caching(enum ttm_caching caching, pgprot_t tmp)
{

	/* Cached mappings need no adjustment. */
	if (caching == ttm_cached)
		return tmp;

	if (caching == ttm_write_combined)
		tmp |= PMAP_WRITE_COMBINE;
	else
		tmp |= PMAP_NOCACHE;

	return tmp;
}
