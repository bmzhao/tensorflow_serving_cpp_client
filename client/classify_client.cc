#include "client/classify_client.h"

#include <memory>
#include <string>

#include "grpcpp/channel.h"
#include "grpcpp/impl/codegen/client_context.h"
#include "grpcpp/impl/codegen/status.h"
#include "tensorflow_serving/apis/classification.pb.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"

namespace tensorflow_serving_client {

using tensorflow::serving::ClassificationRequest;
using tensorflow::serving::ClassificationResponse;
using tensorflow::serving::ClassificationResult;
using tensorflow::serving::ExampleList;
using tensorflow::serving::ModelSpec;
using tensorflow::serving::PredictionService;

ClassifyClient::ClassifyClient(std::shared_ptr<grpc::Channel> channel,
                               const std::string& model_name)
    : model_name_(model_name), stub_(PredictionService::NewStub(channel)) {}

grpc::Status ClassifyClient::Classify(
    const std::string& signature_name,
    const std::vector<tensorflow::Example>& inputs,
    tensorflow::serving::ClassificationResponse* response) {
  ClassificationRequest request;

  // Fill out the model spec.
  ModelSpec* model_spec = request.mutable_model_spec();
  model_spec->set_name(model_name_);
  model_spec->set_signature_name(signature_name);

  ExampleList* example_list = request.mutable_input()->mutable_example_list();
  for (const auto& input : inputs) {
    *example_list->add_examples() = input;
  }
  grpc::ClientContext context;
  return stub_->Classify(&context, request, response);
}

}  // namespace tensorflow_serving_client