// To run this example:
// 1. docker pull tensorflow/serving
// 2. docker run -it --rm -p 8500:8500 -p 8501:8501 \
//    -v "$PWD/examples/models/classify_model:/models/classify_model" \
//    -e MODEL_NAME=classify_model \
//    tensorflow/serving
// 3. bazel run //examples:classify_example

#include <iostream>
#include <memory>
#include <string>

#include "client/classify_client.h"
#include "google/protobuf/map.h"
#include "grpcpp/create_channel.h"
#include "grpcpp/impl/codegen/status.h"
#include "grpcpp/security/credentials.h"
#include "tensorflow/core/example/example.pb.h"
#include "tensorflow/core/example/feature.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow_serving/apis/classification.pb.h"
#include "tensorflow_serving/apis/input.pb.h"
#include "tensorflow_serving/apis/model.pb.h"

using tensorflow::Example;
using tensorflow::Feature;
using tensorflow::FloatList;
using tensorflow::serving::ClassificationResponse;
using tensorflow_serving_client::ClassifyClient;

// Creates a flattened 28x28 float with name "picture"
// with num_ones number of "1.0", and all else zeros.
Example CreateExamplePicture(int num_ones) {
  Example example;
  google::protobuf::Map<std::string, Feature>* features =
      example.mutable_features()->mutable_feature();
  Feature feature;
  FloatList* float_list = feature.mutable_float_list();
  for (int i = 0; i < 28 * 28; ++i) {
    float num = 0.0f;
    if (i < num_ones) {
      num = 1.0f;
    }
    float_list->add_value(num);
  }

  (*features)["picture"] = feature;
  return example;
}

int main() {
  // Default GRPC port is 8500
  // https://www.tensorflow.org/tfx/serving/docker#running_a_serving_image
  std::shared_ptr<grpc::Channel> channel =
      grpc::CreateChannel("localhost:8500", grpc::InsecureChannelCredentials());

  std::string model_name("classify_model");

  ClassifyClient client(channel, model_name);

  // Create 4 pictures:
  // One flattened 28x28 full of ones (hotdog)
  // One flattened 28x28 full of zeros (not hotdog),
  // One flattened 28x28 with exactly 101 ones (hotdog)
  // One flattened 28x28 with exactly 99 ones (not hotdog)
  std::vector<Example> examples;
  examples.push_back(CreateExamplePicture(28 * 28));
  examples.push_back(CreateExamplePicture(0));
  examples.push_back(CreateExamplePicture(101));
  examples.push_back(CreateExamplePicture(99));

  ClassificationResponse response;

  // Call the Classify API
  grpc::Status status =
      client.Classify("classify_signature", examples, &response);

  if (!status.ok()) {
    std::cerr << "Request failure: " << status.error_message();
    return -1;
  }

  std::cout << "Results: \n";
  std::cout << response.DebugString();
}