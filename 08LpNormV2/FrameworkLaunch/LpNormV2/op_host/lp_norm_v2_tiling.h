
#include "register/tilingdata_base.h"

#if 0
namespace optiling {
BEGIN_TILING_DATA_DEF(LpNormV2TilingData)
  TILING_DATA_FIELD_DEF(uint32_t, size);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(LpNormV2, LpNormV2TilingData)
}
#endif

namespace optiling {
BEGIN_TILING_DATA_DEF(LpNormV2TilingData)
TILING_DATA_FIELD_DEF(float, p);
TILING_DATA_FIELD_DEF(uint32_t, axes);
TILING_DATA_FIELD_DEF(bool, keepdim);
TILING_DATA_FIELD_DEF(float, epsilon);

TILING_DATA_FIELD_DEF(uint32_t, dataType);

TILING_DATA_FIELD_DEF(uint32_t, blockLength);
TILING_DATA_FIELD_DEF(uint32_t, tileNum);
TILING_DATA_FIELD_DEF(uint32_t, tileLength);
TILING_DATA_FIELD_DEF(uint32_t, lasttileLength);
TILING_DATA_FIELD_DEF(uint32_t, formerNum);
TILING_DATA_FIELD_DEF(uint32_t, formerLength);
TILING_DATA_FIELD_DEF(uint32_t, formertileNum);
TILING_DATA_FIELD_DEF(uint32_t, formertileLength);
TILING_DATA_FIELD_DEF(uint32_t, formerlasttileLength);
TILING_DATA_FIELD_DEF(uint32_t, tailNum);
TILING_DATA_FIELD_DEF(uint32_t, tailLength);
TILING_DATA_FIELD_DEF(uint32_t, tailtileNum);
TILING_DATA_FIELD_DEF(uint32_t, tailtileLength);
TILING_DATA_FIELD_DEF(uint32_t, taillasttileLength);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(LpNormV2, LpNormV2TilingData)
} // namespace optiling
