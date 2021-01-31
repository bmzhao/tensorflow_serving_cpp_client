#ifndef TENSORFLOW_SERVING_CPP_CLIENT_CLIENT_PREDICT_CLIENT_H_
#define TENSORFLOW_SERVING_CPP_CLIENT_CLIENT_PREDICT_CLIENT_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "grpcpp/channel.h"
#include "grpcpp/impl/codegen/status.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"

namespace tensorflow_serving_client {

class PredictionClient {
 public:
  PredictionClient(std::shared_ptr<grpc::Channel> channel,
                   const std::string& model_name);

  grpc::Status Predict(
      const std::string& signature_name,
      const std::unordered_map<std::string, tensorflow::Tensor>& inputs,
      std::unordered_map<std::string, tensorflow::Tensor>* outputs);

 private:
  std::string model_name_;
  std::unique_ptr<tensorflow::serving::PredictionService::Stub> stub_;
};

}  // namespace tensorflow_serving_client

#endif  // TENSORFLOW_SERVING_CPP_CLIENT_CLIENT_PREDICT_CLIENT_H_