
#include "register/tilingdata_base.h"

#if 0
namespace optiling {
BEGIN_TILING_DATA_DEF(SpenceTilingData)
  TILING_DATA_FIELD_DEF(uint32_t, size);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(Spence, SpenceTilingData)
}
#endif

namespace optiling {
BEGIN_TILING_DATA_DEF(SpenceTilingData)
// TILING_DATA_FIELD_DEF(uint32_t, reduction);
// TILING_DATA_FIELD_DEF(uint32_t, dataTotalLength);
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

REGISTER_TILING_DATA_CLASS(Spence, SpenceTilingData)
} // namespace optiling
