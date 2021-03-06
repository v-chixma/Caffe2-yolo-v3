/**
 * Copyright (c) 2016-present, Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "upsample_op.h"

namespace caffe2 {

REGISTER_CPU_OPERATOR(Upsample, UpsampleOp<float, CPUContext>);
REGISTER_CPU_OPERATOR(
    UpsampleGradient,
    UpsampleGradientOp<float, CPUContext>);

OPERATOR_SCHEMA(Upsample)
    .NumInputs(1)
    .NumOutputs(1)
    .SetDoc(R"DOC(
Nearest neighbor upsampling operation. Implementation taken from THCUNN.
)DOC")
    .Arg(
        "scale",
        "(int) default 2; integer upsampling factor.")
    .Input(
        0,
        "X",
        "4D feature map input of shape (N, C, H, W).")
    .Output(
        0,
        "Y",
        "4D feature map of shape (N, C, scale * H, scale * W); Values are "
        "neareast neighbor samples from X.");

OPERATOR_SCHEMA(UpsampleGradient)
    .NumInputs(2)
    .NumOutputs(1)
    .Input(
        0,
        "X",
        "See Upsample.")
    .Input(
        1,
        "dY",
        "Gradient of forward output 0 (Y).")
    .Output(
        0,
        "dX",
        "Gradient of forward input 0 (X).");

class GetUpsampleGradient : public GradientMakerBase {
  using GradientMakerBase::GradientMakerBase;
  vector<OperatorDef> GetGradientDefs() override {
    return SingleGradientDef(
        "UpsampleGradient",
        "",
        vector<string>{I(0), GO(0)},
        vector<string>{GI(0)});
  }
};

REGISTER_GRADIENT(Upsample, GetUpsampleGradient);

} // namespace caffe2
