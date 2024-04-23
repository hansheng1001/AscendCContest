#ifndef OP_PROTO_H_
#define OP_PROTO_H_

#include "graph/operator_reg.h"
#include "register/op_impl_registry.h"

namespace ge {

REG_OP(FastGeluGrad)
    .INPUT(dy, ge::TensorType::ALL())
    .INPUT(x, ge::TensorType::ALL())
    .OUTPUT(z, ge::TensorType::ALL())
    .OP_END_FACTORY_REG(FastGeluGrad);

}

#endif
