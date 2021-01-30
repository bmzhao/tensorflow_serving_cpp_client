# See https://github.com/bazelbuild/rules_proto/tree/a0761ed101b939e19d83b2da5f59034bffc19c12#getting-started
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_proto",
    sha256 = "32c9deb114c9e2d6ea3afd48a4d203d775b60a01876186d1ad52d752a8be439f",
    strip_prefix = "rules_proto-a0761ed101b939e19d83b2da5f59034bffc19c12",
    urls = [
        "https://github.com/bazelbuild/rules_proto/archive/a0761ed101b939e19d83b2da5f59034bffc19c12.zip",
    ],
)
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

# See https://github.com/grpc/grpc/blob/v1.35.0/src/cpp/README.md#bazel
http_archive(
    name = "com_github_grpc_grpc",
    sha256 = "3c432b6e3ba5eaf8c2593f4e6f61eaf363463eb533557a98a9a9adafc5e0e625",
    urls = [
        "https://github.com/grpc/grpc/archive/v1.35.0.zip",
    ],
    strip_prefix = "grpc-1.35.0",
)
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()
load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()
