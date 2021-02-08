// To run this example:
// 1. docker pull tensorflow/serving
// 2. docker run -it --rm -p 8500:8500 -p 8501:8501 \
//    -v "$PWD/examples/models/x_plus_one:/models/x_plus_one" \
//    -e MODEL_NAME=x_plus_one \
//    tensorflow/serving
// 3. bazel run //examples:predict_http_example

#include <iostream>

#include "httplib.h"
#include "single_include/nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) {
  // httplib API example:
  // https://github.com/yhirose/cpp-httplib/blob/v0.8.3/example/client.cc#L23
  httplib::Client client("http://localhost:8501");
  // JSON API example:
  // https://github.com/nlohmann/json/tree/v3.9.1#json-as-first-class-data-type
  json predict_request;

  // Request format:
  // https://github.com/tensorflow/serving/blob/fec1d5db3cec6c81635e6f77923f3112f09a0289/tensorflow_serving/g3doc/api_rest.md#request-format-1
  predict_request["signature_name"] = "predict";
  predict_request["inputs"]["x"] = 3;

  std::cout << "Sending JSON request :\n"
            << predict_request.dump(4) << std::endl;

  httplib::Result result =
      client.Post("/v1/models/x_plus_one:predict", predict_request.dump(),
                  "application/json");
  if (result.error() != 0) {
    std::cout << "Request failed..." << std::endl;
    return -1;
  }

  std::cout << "Result:\n" << result->body;
}