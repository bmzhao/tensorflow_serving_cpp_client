// To run this example:
// 1. docker pull tensorflow/serving
// 2. docker run -it --rm -p 8500:8500 -p 8501:8501 \
//    -v "$PWD/examples/models/x_plus_one:/models/x_plus_one" \
//    -e MODEL_NAME=x_plus_one \
//    tensorflow/serving
// 3. bazel run //examples:predict_example

#include <iostream>
#include <memory>

#include "client/predict_client.h"
#include "grpcpp/create_channel.h"
#include "grpcpp/impl/codegen/status.h"
#include "grpcpp/security/credentials.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/util/command_line_flags.h"

using tensorflow_serving_client::PredictionClient;

int main() {
  // Default GRPC port is 8500
  // https://www.tensorflow.org/tfx/serving/docker#running_a_serving_image
  std::shared_ptr<grpc::Channel> channel =
      grpc::CreateChannel("localhost:8500", grpc::InsecureChannelCredentials());

  std::string model_name("x_plus_one");

  PredictionClient client(channel, model_name);

  // Create a scalar tensor with value 1, shape {}, dtype=int32.
  // https://github.com/tensorflow/tensorflow/blob/86c79e9779a2d2653fddbb0e0391e2ea8121e81c/tensorflow/core/framework/tensor.h#L190
  tensorflow::Tensor x(1);

  // Create a map of all inputs.
  std::unordered_map<std::string, tensorflow::Tensor> named_inputs;
  named_inputs["x"] = std::move(x);

  // Create a map for outputs;
  std::unordered_map<std::string, tensorflow::Tensor> named_outputs;

  // Call the prediction API
  grpc::Status status = client.Predict("predict", named_inputs, &named_outputs);

  if (!status.ok()) {
    std::cerr << "Request failure: " << status.error_message();
    return -1;
  }

  for (const auto& output_kv : named_outputs) {
    std::cout << "Results: \n";
    std::cout << output_kv.first << ": " << output_kv.second.DebugString();
  }
}