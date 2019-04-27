package(default_visibility = ["//visibility:public"])

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")
load("@mxebzl//tools/windows:rules.bzl", "pkg_winzip")

config_setting(
    name = "windows",
    values = {
        "crosstool_top": "@mxebzl//tools/windows:toolchain",
    }
)

cc_binary(
    name = "ripple",
    data = ["//content"],
    linkopts = select({
        ":windows": ["-mwindows", "-lSDL2main" ],
        "//conditions:default": [],
    }) + [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
        "-static-libstdc++",
        "-static-libgcc",
    ],
    srcs = ["main.cc"],
    deps = [
        "@libgam//:game",
        ":screens",
    ],
)

pkg_winzip(
    name = "ripple-windows",
    files = [
        ":ripple",
        "//content",
    ]
)

pkg_tar(
    name = "ripple-linux",
    extension = "tar.gz",
    strip_prefix = "/",
    package_dir = "ripple/",
    srcs = [
        ":ripple",
        "//content",
    ],
)

cc_library(
    name = "game_state",
    hdrs = ["game_state.h"],
    srcs = ["game_state.cc"],
)

cc_library(
    name = "screens",
    srcs = [
        "title_screen.cc",
        "level_screen.cc",
    ],
    hdrs = [
        "title_screen.h",
        "level_screen.h",
    ],
    deps = [
        "@libgam//:screen",
        "@libgam//:text",
        ":game_state",
        ":map",
        ":player",
    ],
)

cc_library(
    name = "map",
    srcs = ["map.cc"],
    hdrs = ["map.h"],
    deps = [
        "@libgam//:spritemap",
        ":rect",
    ],
)

cc_library(
    name = "rect",
    srcs = ["rect.cc"],
    hdrs = ["rect.h"],
)

cc_library(
    name = "player",
    srcs = ["player.cc"],
    hdrs = ["player.h"],
    deps = [
        "@libgam//:graphics",
        "@libgam//:spritemap",
        ":map",
        ":rect",
    ],
)

