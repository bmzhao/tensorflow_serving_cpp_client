// To run this example:
// 1. docker pull tensorflow/serving
// 2. docker run -it --rm -p 8500:8500 -p 8501:8501 \
//    -v "$PWD/examples/models/regress_model:/models/regress_model" \
//    -e MODEL_NAME=regress_model \
//    tensorflow/serving
// 3. bazel run //examples:regress_http_example

#include <iostream>

#include "httplib.h"
#include "single_include/nlohmann/json.hpp"

using json = nlohmann::json;

json CreatePriceFeature(float price) {
  json result;
  result["price_today"] = json::array();
  result["price_today"].push_back(price);
  return result;
}

int main(int argc, char** argv) {
  // httplib API example:
  // https://github.com/yhirose/cpp-httplib/blob/v0.8.3/example/client.cc#L23
  httplib::Client client("http://localhost:8501");
  // JSON API example:
  // https://github.com/nlohmann/json/tree/v3.9.1#json-as-first-class-data-type
  json regress_request;

  // Request format:
  // https://github.com/tensorflow/serving/blob/fec1d5db3cec6c81635e6f77923f3112f09a0289/tensorflow_serving/g3doc/api_rest.md#request-format
  regress_request["signature_name"] = "regress_signature";
  regress_request["examples"] = json::array();
  regress_request["examples"].push_back(CreatePriceFeature(0));
  regress_request["examples"].push_back(CreatePriceFeature(1));
  regress_request["examples"].push_back(CreatePriceFeature(2));
  regress_request["examples"].push_back(CreatePriceFeature(3));

  std::cout << "Sending JSON request :\n"
            << regress_request.dump(4) << std::endl;

  httplib::Result result =
      client.Post("/v1/models/regress_model:regress", regress_request.dump(),
                  "application/json");
  if (result.error() != 0) {
    std::cout << "Request failed..." << std::endl;
    return -1;
  }

  std::cout << "Result:\n" << result->body;
}