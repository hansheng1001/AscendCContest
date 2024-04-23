
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_MSE_LOSS_GRAD_H_
#define ACLNN_MSE_LOSS_GRAD_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnMseLossGradGetWorkspaceSize
 * parameters :
 * predict : required
 * label : required
 * x : required
 * reductionOptional : optional
 * out : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnMseLossGradGetWorkspaceSize(
    const aclTensor *predict,
    const aclTensor *label,
    const aclTensor *x,
    char *reductionOptional,
    const aclTensor *out,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnMseLossGrad
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnMseLossGrad(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
