cc_binary(
    name = "classify_example",
    srcs = ["classify_example.cc"],
    deps = [
        "//client:classify_client",
        "@com_github_grpc_grpc//:grpc++",
        "@org_tensorflow//tensorflow/core:framework",
        "@org_tensorflow//tensorflow/core:lib",
    ],
)

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

cc_binary(
    name = "regress_example",
    srcs = ["regress_example.cc"],
    deps = [
        "//client:regress_client",
        "@com_github_grpc_grpc//:grpc++",
        "@org_tensorflow//tensorflow/core:framework",
        "@org_tensorflow//tensorflow/core:lib",
    ],
)
