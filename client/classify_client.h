#ifndef TENSORFLOW_SERVING_CPP_CLIENT_CLIENT_CLASSIFY_CLIENT_H_
#define TENSORFLOW_SERVING_CPP_CLIENT_CLIENT_CLASSIFY_CLIENT_H_

#include <memory>
#include <string>
#include <vector>

#include "grpcpp/channel.h"
#include "grpcpp/impl/codegen/status.h"
#include "tensorflow/core/example/example.pb.h"
#include "tensorflow_serving/apis/classification.pb.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"

namespace tensorflow_serving_client {

class ClassifyClient {
 public:
  ClassifyClient(std::shared_ptr<grpc::Channel> channel,
                 const std::string& model_name);

  grpc::Status Classify(const std::string& signature_name,
                        const std::vector<tensorflow::Example>& inputs,
                        tensorflow::serving::ClassificationResponse* response);

 private:
  std::string model_name_;
  std::unique_ptr<tensorflow::serving::PredictionService::Stub> stub_;
};

}  // namespace tensorflow_serving_client

#endif  // TENSORFLOW_SERVING_CPP_CLIENT_CLIENT_CLASSIFY_CLIENT_H_