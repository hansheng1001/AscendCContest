#ifndef OP_PROTO_H_
#define OP_PROTO_H_

#include "graph/operator_reg.h"
#include "register/op_impl_registry.h"

namespace ge {

REG_OP(LpNormV2)
    .INPUT(x, ge::TensorType::ALL())
    .OUTPUT(y, ge::TensorType::ALL())
    .ATTR(p, Float, 2)
    .ATTR(axes, ListInt, {})
    .ATTR(keepdim, Bool, false)
    .ATTR(epsilon, Float, 1e-12)
    .OP_END_FACTORY_REG(LpNormV2);

}

#endif
