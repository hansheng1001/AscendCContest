#include <string.h>
#include "graph/types.h"
#include "aclnn_scatter_sub.h"

namespace {
typedef struct {
    uint32_t id;
    const char *funcName;
    bool hasReg;
} NnopbaseDfxId;
typedef struct {
    ge::DataType dtype;
    ge::Format format;
} TensorDesc;
typedef struct {
    TensorDesc *inputsDesc;
    size_t inputsNum;
    TensorDesc *outputsDesc;
    size_t outputsNum;
} SupportInfo;
typedef struct {
    SupportInfo *supportInfo;
    size_t num;
} OpSocSupportInfo;
typedef struct {
    OpSocSupportInfo *socSupportInfo;
    size_t num;
} OpSupportList;
enum SocType {
    SOC_VERSION_ASCEND910A = 1,
    SOC_VERSION_ASCEND910B,
    SOC_VERSION_ASCEND910C,
    SOC_VERSION_ASCEND310P,
    SOC_VERSION_ASCEND310B,
    SOC_VERSION_BS9SX1A
};
uint32_t socSupportList[] = {SOC_VERSION_ASCEND310B};
uint32_t socSupportListLen = 1;

TensorDesc inputDesc0_0[3] =
    {{ge::DT_FLOAT16, ge::FORMAT_ND},
     {ge::DT_INT32, ge::FORMAT_ND},
     {ge::DT_FLOAT16, ge::FORMAT_ND}};
TensorDesc inputDesc0_1[3] =
    {{ge::DT_FLOAT, ge::FORMAT_ND},
     {ge::DT_INT32, ge::FORMAT_ND},
     {ge::DT_FLOAT, ge::FORMAT_ND}};
TensorDesc inputDesc0_2[3] =
    {{ge::DT_INT32, ge::FORMAT_ND},
     {ge::DT_INT32, ge::FORMAT_ND},
     {ge::DT_INT32, ge::FORMAT_ND}};
TensorDesc inputDesc0_3[3] =
    {{ge::DT_INT8, ge::FORMAT_ND},
     {ge::DT_INT32, ge::FORMAT_ND},
     {ge::DT_INT8, ge::FORMAT_ND}};
SupportInfo list0_0 = {inputDesc0_0, 3};
SupportInfo list0_1 = {inputDesc0_1, 3};
SupportInfo list0_2 = {inputDesc0_2, 3};
SupportInfo list0_3 = {inputDesc0_3, 3};
SupportInfo supportInfo0[4] = {list0_0, list0_1, list0_2, list0_3};
OpSocSupportInfo socSupportInfo0= {supportInfo0, 4};

OpSocSupportInfo opSocSupportList[1] = {socSupportInfo0};
OpSupportList supportList = {opSocSupportList, 1};

[[maybe_unused]] uint32_t NNOPBASE_ScatterSub = 0U;
} // namespace

extern void NnopbaseOpLogE(const aclnnStatus code, const char *const expr);

#ifdef __cplusplus
extern "C" {
#endif

extern aclnnStatus NnopbaseCreateExecutorSpace(void **space);
extern void *NnopbaseGetExecutor(void *space, const char *opType, char *inputsDesc, uint32_t inputNum,
                                 char *outputsDesc, uint32_t outputNum, char *attrsDesc, uint32_t attrsNum);
extern aclnnStatus NnopbaseAddInput(void *executor, const aclTensor *tensor, const uint32_t index);
extern aclnnStatus NnopbaseAddIgnoreContinuesInput(void *executor,
                                                   const aclTensor *tensor, const uint32_t index);
extern aclnnStatus NnopbaseAddIntArrayInput(void *executor, const aclIntArray *array, const uint32_t index);
extern aclnnStatus NnopbaseAddBoolArrayInput(void *executor, const aclBoolArray *array, const uint32_t index);
extern aclnnStatus NnopbaseAddFloatArrayInput(void *executor, const aclFloatArray *array, const uint32_t index);
extern aclnnStatus NnopbaseAddOutput(void *executor, const aclTensor *tensor, const uint32_t index);
extern aclnnStatus NnopbaseAddDynamicInput(void *executor, const aclTensorList *tensor_list, const uint32_t index);
extern aclnnStatus NnopbaseAddDynamicOutput(void *executor, const aclTensorList *tensor_list, const uint32_t index);
extern aclnnStatus NnopbaseAddAttr(void *executor, void *attrAddr, size_t attrLen, const size_t index);
extern aclnnStatus NnopbaseAddIntArrayAttr(void *executor, const aclIntArray* array, const size_t index);
extern aclnnStatus NnopbaseAddFloatArrayAttr(void *executor, const aclFloatArray* array, const size_t index);
extern aclnnStatus NnopbaseAddBoolArrayAttr(void *executor, const aclBoolArray* array, const size_t index);
extern aclnnStatus NnopbaseAddArrayAttr(void *executor, void *array, const size_t len, const size_t elementSize, const size_t index);
extern uint64_t NnopbaseMsprofSysTime();
extern aclnnStatus NnopbaseAddTilingId(void *executor, NnopbaseDfxId *tilingId);
extern void NnopbaseReportApiInfo(const uint64_t beginTime, NnopbaseDfxId &dfxId);
extern aclnnStatus NnopbaseRunForWorkspace(void *executor, uint64_t *workspaceLen);
extern aclnnStatus NnopbaseRunWithWorkspace(void *executor, aclrtStream stream, void *workspace, uint64_t workspaceSize);
extern aclnnStatus NnopbaseAddSupportList(void *executor, OpSupportList *list, uint32_t *socSupportList, size_t socSupportListLen);
extern aclnnStatus NnopbaseAddScalarInput(void *executor, const aclScalar *scalar, const uint32_t index, const int32_t srcIndex, const ge::DataType dtype);
extern aclnnStatus NnopbaseAddScalarListInput(void *executor, const aclScalarList *scalarList, const uint32_t index, const int32_t srcIndex, const ge::DataType dtype);

#define ACLNN_SUCCESS  0
#define ACLNN_ERR_PARAM_NULLPTR 161001

#define NNOPBASE_ASSERT_OK_RETVAL(v)                                    \
    do {                                                                \
        const aclnnStatus _chk_stutus = (v);                            \
        if (_chk_stutus != ACLNN_SUCCESS) {                             \
            NnopbaseOpLogE(_chk_stutus, #v);                            \
            return _chk_stutus;                                         \
        }                                                               \
    } while (false)

#define NNOPBASE_ASSERT_NOTNULL_RETVAL(v)                               \
    do {                                                                \
        if ((v) == nullptr) {                                           \
            NnopbaseOpLogE(ACLNN_ERR_PARAM_NULLPTR, #v " != nullptr");  \
            return ACLNN_ERR_PARAM_NULLPTR;                             \
        }                                                               \
    } while (false)

aclnnStatus aclnnScatterSubGetWorkspaceSize(
    const aclTensor *x,
    const aclTensor *index,
    const aclTensor *update,
    bool useLockingOptional,
    uint64_t *workspaceSize,
    aclOpExecutor **executor)
{
    uint64_t timeStamp = NnopbaseMsprofSysTime();
    static NnopbaseDfxId dfxId = {0x60000, __func__, false};
    static NnopbaseDfxId tilingId = {0x60000, "aclnnScatterSubTiling", false};
    void *nnopExecutor;
    static void *executorSpace = NULL;
    const char *opType = "ScatterSub";
    char inputDesc[] = {1, 1, 1};
    char outputDesc[] = {};
    char attrDesc[] = {0};

    NNOPBASE_ASSERT_NOTNULL_RETVAL(x);
    NNOPBASE_ASSERT_NOTNULL_RETVAL(index);
    NNOPBASE_ASSERT_NOTNULL_RETVAL(update);

    if (!executorSpace) {
        NNOPBASE_ASSERT_OK_RETVAL(NnopbaseCreateExecutorSpace(&executorSpace));
    }
    nnopExecutor = NnopbaseGetExecutor(executorSpace, opType, inputDesc, sizeof(inputDesc) / sizeof(char), outputDesc,
                                       sizeof(outputDesc) / sizeof(char), attrDesc, sizeof(attrDesc) / sizeof(char));
    NNOPBASE_ASSERT_NOTNULL_RETVAL(nnopExecutor);
    NNOPBASE_ASSERT_NOTNULL_RETVAL(executor);
    *executor = reinterpret_cast<aclOpExecutor *>(nnopExecutor);
    NNOPBASE_ASSERT_OK_RETVAL(NnopbaseAddTilingId(*executor, &tilingId));
    NNOPBASE_ASSERT_OK_RETVAL(NnopbaseAddInput(*executor, x, 0));
    NNOPBASE_ASSERT_OK_RETVAL(NnopbaseAddInput(*executor, index, 1));
    NNOPBASE_ASSERT_OK_RETVAL(NnopbaseAddInput(*executor, update, 2));
    NNOPBASE_ASSERT_OK_RETVAL(NnopbaseAddAttr(*executor, static_cast<void*>(&useLockingOptional), sizeof(bool), 0));
    NNOPBASE_ASSERT_OK_RETVAL(NnopbaseAddSupportList(*executor, &supportList, socSupportList, socSupportListLen));
    aclnnStatus ret = NnopbaseRunForWorkspace(*executor, workspaceSize);
    NnopbaseReportApiInfo(timeStamp, dfxId);
    return ret;
}

aclnnStatus aclnnScatterSub(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    const aclrtStream stream)
{
    uint64_t timeStamp = NnopbaseMsprofSysTime();
    static NnopbaseDfxId dfxId = {0x60000, __func__, false};
    aclnnStatus ret = NnopbaseRunWithWorkspace(executor, stream, workspace, workspaceSize);
    NnopbaseReportApiInfo(timeStamp, dfxId);
    return ret;
}

#ifdef __cplusplus
}
#endif
