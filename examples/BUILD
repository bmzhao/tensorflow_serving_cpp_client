cc_binary(
    name = "predict_example",
    srcs = ["predict_example.cc"],
    deps = [
        "//client:predict_client",
        "@com_github_grpc_grpc//:grpc++",
        "@org_tensorflow//tensorflow/core:framework",
        "@org_tensorflow//tensorflow/core:lib",
    ],
)
