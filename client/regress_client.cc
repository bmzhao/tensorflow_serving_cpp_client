#include "client/regress_client.h"

#include <memory>
#include <string>

#include "grpcpp/channel.h"
#include "grpcpp/impl/codegen/client_context.h"
#include "grpcpp/impl/codegen/status.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"
#include "tensorflow_serving/apis/regression.pb.h"

namespace tensorflow_serving_client {

using tensorflow::serving::ExampleList;
using tensorflow::serving::ModelSpec;
using tensorflow::serving::PredictionService;
using tensorflow::serving::Regression;
using tensorflow::serving::RegressionRequest;
using tensorflow::serving::RegressionResponse;
using tensorflow::serving::RegressionResult;

RegressClient::RegressClient(std::shared_ptr<grpc::Channel> channel,
                             const std::string& model_name)
    : model_name_(model_name), stub_(PredictionService::NewStub(channel)) {}

grpc::Status RegressClient::Regress(
    const std::string& signature_name,
    const std::vector<tensorflow::Example>& inputs,
    std::vector<float>* results) {
  RegressionRequest request;

  // Fill out the model spec.
  ModelSpec* model_spec = request.mutable_model_spec();
  model_spec->set_name(model_name_);
  model_spec->set_signature_name(signature_name);

  ExampleList* example_list = request.mutable_input()->mutable_example_list();
  for (const auto& input : inputs) {
    *example_list->add_examples() = input;
  }
  grpc::ClientContext context;
  RegressionResponse response;
  grpc::Status status = stub_->Regress(&context, request, &response);
  if (!status.ok()) {
    return status;
  }

  const RegressionResult& regression_result = response.result();
  results->clear();
  results->reserve(regression_result.regressions_size());

  for (const Regression& regression : regression_result.regressions()) {
    results->push_back(regression.value());
  }
  return status;
}

}  // namespace tensorflow_serving_client