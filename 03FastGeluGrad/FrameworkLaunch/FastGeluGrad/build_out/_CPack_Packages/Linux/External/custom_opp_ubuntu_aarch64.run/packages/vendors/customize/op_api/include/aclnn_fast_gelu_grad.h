
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_FAST_GELU_GRAD_H_
#define ACLNN_FAST_GELU_GRAD_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnFastGeluGradGetWorkspaceSize
 * parameters :
 * dy : required
 * x : required
 * out : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnFastGeluGradGetWorkspaceSize(
    const aclTensor *dy,
    const aclTensor *x,
    const aclTensor *out,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnFastGeluGrad
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnFastGeluGrad(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
