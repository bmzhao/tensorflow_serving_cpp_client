#ifndef TENSORFLOW_SERVING_CPP_CLIENT_CLIENT_REGRESS_CLIENT_H_
#define TENSORFLOW_SERVING_CPP_CLIENT_CLIENT_REGRESS_CLIENT_H_

#include <memory>
#include <string>
#include <vector>

#include "grpcpp/channel.h"
#include "grpcpp/impl/codegen/status.h"
#include "tensorflow/core/example/example.pb.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"
#include "tensorflow_serving/apis/regression.pb.h"

namespace tensorflow_serving_client {

class RegressClient {
 public:
  RegressClient(std::shared_ptr<grpc::Channel> channel,
                const std::string& model_name);

  grpc::Status Regress(const std::string& signature_name,
                       const std::vector<tensorflow::Example>& inputs,
                       std::vector<float>* results);

 private:
  std::string model_name_;
  std::unique_ptr<tensorflow::serving::PredictionService::Stub> stub_;
};

}  // namespace tensorflow_serving_client

#endif  // TENSORFLOW_SERVING_CPP_CLIENT_CLIENT_REGRESS_CLIENT_H_