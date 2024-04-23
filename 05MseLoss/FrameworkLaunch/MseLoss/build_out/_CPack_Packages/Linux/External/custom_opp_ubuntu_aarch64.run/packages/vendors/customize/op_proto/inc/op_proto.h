#ifndef OP_PROTO_H_
#define OP_PROTO_H_

#include "graph/operator_reg.h"
#include "register/op_impl_registry.h"

namespace ge {

REG_OP(MseLoss)
    .INPUT(x, ge::TensorType::ALL())
    .INPUT(y, ge::TensorType::ALL())
    .OUTPUT(out, ge::TensorType::ALL())
    .ATTR(reduction, String, "mean")
    .OP_END_FACTORY_REG(MseLoss);

}

#endif
