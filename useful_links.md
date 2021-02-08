# GRPC Example Client Code C++
* [Github](https://github.com/grpc/grpc/tree/master/examples/cpp/helloworld)
* [Sync Client](https://github.com/grpc/grpc/blob/c7e49549750064535842d86b96a2c2a94eb9f0a0/examples/cpp/helloworld/greeter_client.cc#L38)
* [Async Client](https://github.com/grpc/grpc/blob/c7e49549750064535842d86b96a2c2a94eb9f0a0/examples/cpp/helloworld/greeter_async_client.cc#L48)


# Existing TF Serving Client Example C++
* [Resnet Client](https://github.com/tensorflow/serving/blob/14155fe2d4fdedb79b54a2178ee35584beb63cac/tensorflow_serving/example/resnet_client.cc#L37)

# SavedModel Signatures
* [SignatureDef contracts for Predict/Classify/Regress](https://www.tensorflow.org/tfx/serving/signature_defs#sample_structures)
* [Classify Signaturedef Code](https://github.com/tensorflow/tensorflow/blob/86c79e9779a2d2653fddbb0e0391e2ea8121e81c/tensorflow/python/saved_model/signature_def_utils_impl.py#L119)
* TF Serving Additional Classify Requirements: [Pre](https://github.com/tensorflow/serving/blob/b70a0d28f48550380c1996709abc36089d135f31/tensorflow_serving/servables/tensorflow/classifier.cc#L175) + [Post](https://github.com/tensorflow/serving/blob/b70a0d28f48550380c1996709abc36089d135f31/tensorflow_serving/servables/tensorflow/classifier.cc#L255)
* [Regress Signaturedef Code](https://github.com/tensorflow/tensorflow/blob/86c79e9779a2d2653fddbb0e0391e2ea8121e81c/tensorflow/python/saved_model/signature_def_utils_impl.py#L71)
* TF Serving Additional Regress Requirements: [Pre](https://github.com/tensorflow/serving/blob/b70a0d28f48550380c1996709abc36089d135f31/tensorflow_serving/servables/tensorflow/regressor.cc#L169) + [Post](https://github.com/tensorflow/serving/blob/b70a0d28f48550380c1996709abc36089d135f31/tensorflow_serving/servables/tensorflow/regressor.cc#L209)
* [Predict Signaturedef Code](https://github.com/tensorflow/tensorflow/blob/86c79e9779a2d2653fddbb0e0391e2ea8121e81c/tensorflow/python/saved_model/signature_def_utils_impl.py#L178)
* [Signaturedef String Constants](https://github.com/tensorflow/tensorflow/blob/86c79e9779a2d2653fddbb0e0391e2ea8121e81c/tensorflow/cc/saved_model/signature_constants.h#L19)
* [Run ModelServer with Docker](https://www.tensorflow.org/tfx/serving/docker)


# Tensorflow Serving HTTP API
* [REST API Docs](https://github.com/tensorflow/serving/blob/master/tensorflow_serving/g3doc/api_rest.md#restful-api)

# Bazel Dependencies
* [Design for External Deps](https://github.com/bazelbuild/proposals/blob/master/designs/2018-11-07-design-recursive-workspaces.md#recursive-workspaces)
* [Example Workspace File for Deps on GRPC and Proto](https://github.com/bmzhao/tensorflow_serving_cpp_client/blob/df18746ac569fa165758d1df55b65d313383ec88/WORKSPACE)
* [Proto Deps Example](https://github.com/bazelbuild/rules_proto/tree/a0761ed101b939e19d83b2da5f59034bffc19c12#getting-started)
* [GRPC Deps example](https://github.com/grpc/grpc/blob/v1.35.0/src/cpp/README.md#bazel)
* [TF Serving Workspace](https://github.com/tensorflow/serving/blob/2.4.1/WORKSPACE)
* [GCC stringop-truncation error](https://github.com/bazelbuild/bazel/issues/12056)


# RPC Service Definitions
* [PredictionService](https://github.com/tensorflow/serving/blob/b2005f5548b1267572a84b72d499d0cc2a9ed47a/tensorflow_serving/apis/prediction_service.proto#L15)
* [ModelMetadata](https://github.com/tensorflow/serving/blob/b2005f5548b1267572a84b72d499d0cc2a9ed47a/tensorflow_serving/apis/get_model_metadata.proto#L15)
* [Predict](https://github.com/tensorflow/serving/blob/b2005f5548b1267572a84b72d499d0cc2a9ed47a/tensorflow_serving/apis/predict.proto#L12)
* [Regress](https://github.com/tensorflow/serving/blob/b2005f5548b1267572a84b72d499d0cc2a9ed47a/tensorflow_serving/apis/regression.proto#L23)
* [Classify](https://github.com/tensorflow/serving/blob/b2005f5548b1267572a84b72d499d0cc2a9ed47a/tensorflow_serving/apis/classification.proto#L33)