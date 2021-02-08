// To run this example:
// 1. docker pull tensorflow/serving
// 2. docker run -it --rm -p 8500:8500 -p 8501:8501 \
//    -v "$PWD/examples/models/classify_model:/models/classify_model" \
//    -e MODEL_NAME=classify_model \
//    tensorflow/serving
// 3. bazel run //examples:classify_http_example

#include <iostream>

#include "httplib.h"
#include "single_include/nlohmann/json.hpp"

using json = nlohmann::json;

json CreateExampleFeature(int num_ones) {
  json feature = json::array();
  for (int i = 0; i < 28 * 28; ++i) {
    float num = 0.0f;
    if (i < num_ones) {
      num = 1.0f;
    }
    feature.push_back(num);
  }
  json result;
  result["picture"] = feature;
  return result;
}

int main(int argc, char** argv) {
  // httplib API example:
  // https://github.com/yhirose/cpp-httplib/blob/v0.8.3/example/client.cc#L23
  httplib::Client client("http://localhost:8501");
  // JSON API example:
  // https://github.com/nlohmann/json/tree/v3.9.1#json-as-first-class-data-type
  json classify_request;

  // Request format:
  // https://github.com/tensorflow/serving/blob/fec1d5db3cec6c81635e6f77923f3112f09a0289/tensorflow_serving/g3doc/api_rest.md#request-format
  classify_request["signature_name"] = "classify_signature";
  classify_request["examples"] = json::array();
  // Not Hotdog
  classify_request["examples"].push_back(CreateExampleFeature(0));
  // Hotdog
  classify_request["examples"].push_back(CreateExampleFeature(101));
  // Not Hotdog
  classify_request["examples"].push_back(CreateExampleFeature(99));
  // Hotdog
  classify_request["examples"].push_back(CreateExampleFeature(128));

  std::cout << "Sending JSON request :\n"
            << classify_request.dump(4).substr(0, 500) << "..." << std::endl;

  httplib::Result result =
      client.Post("/v1/models/classify_model:classify", classify_request.dump(),
                  "application/json");
  if (result.error() != 0) {
    std::cout << "Request failed..." << std::endl;
    return -1;
  }

  std::cout << "Result:\n" << result->body;
}