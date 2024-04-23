
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_MSE_LOSS_H_
#define ACLNN_MSE_LOSS_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnMseLossGetWorkspaceSize
 * parameters :
 * x : required
 * y : required
 * reductionOptional : optional
 * out : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnMseLossGetWorkspaceSize(
    const aclTensor *x,
    const aclTensor *y,
    char *reductionOptional,
    const aclTensor *out,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnMseLoss
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnMseLoss(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
