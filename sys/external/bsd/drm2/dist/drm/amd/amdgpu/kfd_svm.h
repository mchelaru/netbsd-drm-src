/*	$NetBSD$	*/

/*
 * amdgpu sources include "kfd_svm.h" with a quoted path.  The real
 * header lives under amdkfd/; wrap it so the amdgpu -I search finds it.
 * When CONFIG_HSA_AMD_SVM is unset, that header provides inline stubs.
 */

#include "../amdkfd/kfd_svm.h"
