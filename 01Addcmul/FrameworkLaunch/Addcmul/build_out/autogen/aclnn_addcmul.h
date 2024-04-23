
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_ADDCMUL_H_
#define ACLNN_ADDCMUL_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnAddcmulGetWorkspaceSize
 * parameters :
 * value : required
 * inputData : required
 * x1 : required
 * x2 : required
 * out : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnAddcmulGetWorkspaceSize(
    const aclTensor *value,
    const aclTensor *inputData,
    const aclTensor *x1,
    const aclTensor *x2,
    const aclTensor *out,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnAddcmul
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnAddcmul(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
