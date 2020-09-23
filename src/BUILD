load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "kalam",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":editor",
        ":logger",
        ":terminal",
    ],
)

cc_library(
    name = "terminal",
    hdrs = ["terminal.h"],
    deps = ["logger"],
)

cc_library(
    name = "logger",
    hdrs = ["logger.h"],
)

cc_library(
    name = "editor",
    srcs = ["editor.cc"],
    hdrs = ["editor.h"],
    deps = [":terminal"],
)
