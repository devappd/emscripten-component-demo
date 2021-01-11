cmake_minimum_required(VERSION 3.00)

set(PROXY_TO_WORKER OFF CACHE BOOL "Build WASM as a web worker.")

project(HelloTriangle
    VERSION "1.0.0"
    LANGUAGES CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(HelloTriangle
    src/hello_triangle.cpp
    src/camera.cpp
    src/events.cpp
)

set_target_properties(HelloTriangle PROPERTIES
    OUTPUT_NAME "wasm-test"
    SUFFIX ".js"
)

target_compile_options(HelloTriangle PRIVATE
    "SHELL:-s USE_SDL=2 -s FULL_ES2=1")

if(PROXY_TO_WORKER)
    target_link_options(HelloTriangle PRIVATE
        "SHELL:-s USE_SDL=2 -s FULL_ES2=1 \
            --proxy-to-worker \
            -s ENVIRONMENT='worker' \
            -s ASYNCIFY=1 \
        "
    )
else()
    target_link_options(HelloTriangle PRIVATE
        "SHELL:-s USE_SDL=2 -s FULL_ES2=1 \
            -s MODULARIZE=1 \
            -s ENVIRONMENT='web' \
            -s ASYNCIFY=1 \
            -s EXTRA_EXPORTED_RUNTIME_METHODS=\"[ \
                'specialHTMLTargets', \
                'JSEvents', \
                'GL', \
                'callMain', \
                'abort' \
            ]\""
    )
endif()

# Install program binaries and preloaded data
install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
    FILES_MATCHING
        PATTERN "*.data"
        PATTERN "*.html"
        PATTERN "*.js"
        PATTERN "*.worker.js"
        PATTERN "*.mjs"
        PATTERN "*.wasm"
        PATTERN "*.a"
)
