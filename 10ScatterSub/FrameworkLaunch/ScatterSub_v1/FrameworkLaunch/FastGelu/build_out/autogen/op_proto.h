#ifndef OP_PROTO_H_
#define OP_PROTO_H_

#include "graph/operator_reg.h"
#include "register/op_impl_registry.h"

namespace ge {

REG_OP(FastGelu)
    .INPUT(x, ge::TensorType::ALL())
    .OUTPUT(y, ge::TensorType::ALL())
    .OP_END_FACTORY_REG(FastGelu);

}

#endif
