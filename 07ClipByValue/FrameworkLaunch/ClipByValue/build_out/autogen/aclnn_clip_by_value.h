
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_CLIP_BY_VALUE_H_
#define ACLNN_CLIP_BY_VALUE_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnClipByValueGetWorkspaceSize
 * parameters :
 * x : required
 * minv : required
 * maxv : required
 * out : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnClipByValueGetWorkspaceSize(
    const aclTensor *x,
    const aclTensor *minv,
    const aclTensor *maxv,
    const aclTensor *out,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnClipByValue
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnClipByValue(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
