#include "client/predict_client.h"

#include <memory>
#include <string>
#include <unordered_map>

#include "google/protobuf/map.h"
#include "grpcpp/channel.h"
#include "grpcpp/impl/codegen/client_context.h"
#include "grpcpp/impl/codegen/status.h"
#include "grpcpp/impl/codegen/status_code_enum.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow_serving/apis/model.pb.h"
#include "tensorflow_serving/apis/predict.pb.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"

namespace tensorflow_serving_client {

using tensorflow::Tensor;
using tensorflow::TensorProto;
using tensorflow::serving::ModelSpec;
using tensorflow::serving::PredictionService;
using tensorflow::serving::PredictRequest;
using tensorflow::serving::PredictResponse;

PredictionClient::PredictionClient(std::shared_ptr<grpc::Channel> channel,
                                   const std::string& model_name)
    : model_name_(model_name), stub_(PredictionService::NewStub(channel)) {}

grpc::Status PredictionClient::Predict(
    const std::string& signature_name,
    const std::unordered_map<std::string, tensorflow::Tensor>& inputs,
    std::unordered_map<std::string, tensorflow::Tensor>* outputs) {
  PredictRequest request;
  PredictResponse response;

  // Fill out the model spec.
  ModelSpec* model_spec = request.mutable_model_spec();
  model_spec->set_name(model_name_);
  model_spec->set_signature_name(signature_name);

  // Copy over inputs into request proto.
  google::protobuf::Map<std::string, TensorProto>* request_inputs =
      request.mutable_inputs();
  for (const auto& input_kv : inputs) {
    const std::string& input_name = input_kv.first;
    const Tensor& tensor = input_kv.second;
    TensorProto proto;
    tensor.AsProtoTensorContent(&proto);
    (*request_inputs)[input_name] = std::move(proto);
  }

  grpc::ClientContext context;
  grpc::Status status = stub_->Predict(&context, request, &response);
  if (!status.ok()) {
    return status;
  }

  for (const auto& output_kv : response.outputs()) {
    const std::string& output_name = output_kv.first;
    const TensorProto& proto = output_kv.second;
    Tensor output_tensor;
    if (!output_tensor.FromProto(proto)) {
      return grpc::Status(grpc::INTERNAL,
                          "Failed to parse output tensor proto");
    }
    (*outputs)[output_name] = std::move(output_tensor);
  }

  return status;
}

}  // namespace tensorflow_serving_client