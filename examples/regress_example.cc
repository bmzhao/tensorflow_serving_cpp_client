// To run this example:
// 1. docker pull tensorflow/serving
// 2. docker run -it --rm -p 8500:8500 -p 8501:8501 \
//    -v "$PWD/examples/models/regress_model:/models/regress_model" \
//    -e MODEL_NAME=regress_model \
//    tensorflow/serving
// 3. bazel run //examples:regress_example

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "client/regress_client.h"
#include "google/protobuf/map.h"
#include "grpcpp/create_channel.h"
#include "grpcpp/impl/codegen/status.h"
#include "grpcpp/security/credentials.h"
#include "tensorflow/core/example/example.pb.h"
#include "tensorflow/core/example/feature.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow_serving/apis/input.pb.h"
#include "tensorflow_serving/apis/model.pb.h"

using tensorflow::Example;
using tensorflow::Feature;
using tensorflow::FloatList;
using tensorflow_serving_client::RegressClient;

// Creates an example consisting of a scalar float with
// name "price_today" with value `value`
Example CreateExampleFloat(float value) {
  Example example;
  google::protobuf::Map<std::string, Feature>* features =
      example.mutable_features()->mutable_feature();
  Feature feature;
  FloatList* float_list = feature.mutable_float_list();
  float_list->add_value(value);

  (*features)["price_today"] = feature;
  return example;
}

int main() {
  // Default GRPC port is 8500
  // https://www.tensorflow.org/tfx/serving/docker#running_a_serving_image
  std::shared_ptr<grpc::Channel> channel =
      grpc::CreateChannel("localhost:8500", grpc::InsecureChannelCredentials());

  std::string model_name("regress_model");

  RegressClient client(channel, model_name);

  // Create 4 toy prices, from $0 to $4.
  // The linear model should estimate a future price of 2x + 3
  std::vector<Example> examples;
  examples.push_back(CreateExampleFloat(0.0f));
  examples.push_back(CreateExampleFloat(1.0f));
  examples.push_back(CreateExampleFloat(2.0f));
  examples.push_back(CreateExampleFloat(3.0f));
  examples.push_back(CreateExampleFloat(4.0f));

  std::vector<float> response;

  // Call the Classify API
  grpc::Status status =
      client.Regress("regress_signature", examples, &response);

  if (!status.ok()) {
    std::cerr << "Request failure: " << status.error_message();
    return -1;
  }

  // The results should be:
  // 3.0, 5.0, 7.0, 9.0, 11.0
  std::cout << "Results: \n";
  for (float result : response) {
    std::cout << result << "\n";
  }
}