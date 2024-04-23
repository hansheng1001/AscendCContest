
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_SCATTER_SUB_H_
#define ACLNN_SCATTER_SUB_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnScatterSubGetWorkspaceSize
 * parameters :
 * x : required
 * index : required
 * update : required
 * useLockingOptional : optional
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnScatterSubGetWorkspaceSize(
    const aclTensor *x,
    const aclTensor *index,
    const aclTensor *update,
    bool useLockingOptional,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnScatterSub
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnScatterSub(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
