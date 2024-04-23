
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_SCATTER_MAX_H_
#define ACLNN_SCATTER_MAX_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnScatterMaxGetWorkspaceSize
 * parameters :
 * xRef : required
 * index : required
 * update : required
 * useLockingOptional : optional
 * xRef : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnScatterMaxGetWorkspaceSize(
    aclTensor *xRef,
    const aclTensor *index,
    const aclTensor *update,
    bool useLockingOptional,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnScatterMax
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnScatterMax(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
