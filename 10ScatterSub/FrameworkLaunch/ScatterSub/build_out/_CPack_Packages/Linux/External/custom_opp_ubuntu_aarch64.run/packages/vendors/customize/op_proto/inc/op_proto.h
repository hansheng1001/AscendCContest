#ifndef OP_PROTO_H_
#define OP_PROTO_H_

#include "graph/operator_reg.h"
#include "register/op_impl_registry.h"

namespace ge {

REG_OP(ScatterSub)
    .INPUT(x, ge::TensorType::ALL())
    .INPUT(index, ge::TensorType::ALL())
    .INPUT(update, ge::TensorType::ALL())
    .OUTPUT(x, ge::TensorType::ALL())
    .ATTR(use_locking, Bool, false)
    .OP_END_FACTORY_REG(ScatterSub);

}

#endif
