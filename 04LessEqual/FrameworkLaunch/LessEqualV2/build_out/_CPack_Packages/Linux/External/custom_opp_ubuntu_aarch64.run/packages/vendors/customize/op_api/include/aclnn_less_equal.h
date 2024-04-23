
/*
 * calution: this file was generated automaticlly donot change it.
*/

#ifndef ACLNN_LESS_EQUAL_H_
#define ACLNN_LESS_EQUAL_H_

#include "aclnn/acl_meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* funtion: aclnnLessEqualGetWorkspaceSize
 * parameters :
 * x1 : required
 * x2 : required
 * out : required
 * workspaceSize : size of workspace(output).
 * executor : executor context(output).
 */
__attribute__((visibility("default")))
aclnnStatus aclnnLessEqualGetWorkspaceSize(
    const aclTensor *x1,
    const aclTensor *x2,
    const aclTensor *out,
    uint64_t *workspaceSize,
    aclOpExecutor **executor);

/* funtion: aclnnLessEqual
 * parameters :
 * workspace : workspace memory addr(input).
 * workspaceSize : size of workspace(input).
 * executor : executor context(input).
 * stream : acl stream.
 */
__attribute__((visibility("default")))
aclnnStatus aclnnLessEqual(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream);

#ifdef __cplusplus
}
#endif

#endif
