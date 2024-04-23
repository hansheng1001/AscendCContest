#ifndef OP_PROTO_H_
#define OP_PROTO_H_

#include "graph/operator_reg.h"
#include "register/op_impl_registry.h"

namespace ge {

REG_OP(LessEqual)
    .INPUT(x1, ge::TensorType::ALL())
    .INPUT(x2, ge::TensorType::ALL())
    .OUTPUT(y, ge::TensorType::ALL())
    .OP_END_FACTORY_REG(LessEqual);

}

#endif
