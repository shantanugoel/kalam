load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "kalam",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":editor",
        ":editor_state",
        ":logger",
        ":terminal",
    ],
)

cc_library(
    name = "terminal",
    srcs = ["terminal.cc"],
    hdrs = ["terminal.h"],
    deps = [
        ":logger",
        ":utils",
    ],
)

cc_library(
    name = "logger",
    hdrs = ["logger.h"],
)

cc_library(
    name = "editor",
    srcs = ["editor.cc"],
    hdrs = ["editor.h"],
    deps = [
        ":editor_state",
        ":terminal",
        ":utils",
    ],
)

cc_library(
    name = "editor_state",
    hdrs = ["editor_state.h"],
)

cc_library(
    name = "utils",
    hdrs = ["utils.h"],
)
