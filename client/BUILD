cc_library(
    name = "predict_client",
    srcs = ["predict_client.cc"],
    hdrs = ["predict_client.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_grpc_grpc//:grpc++",
        "@com_google_protobuf//:protobuf_lite",
        "@org_tensorflow//tensorflow/core:framework",
        "@org_tensorflow//tensorflow/core:lib",
        "@org_tensorflow_serving//tensorflow_serving/apis:prediction_service_cc_proto",
    ],
)
