# 拷贝资源文件到构建目录和运行目录
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_CURRENT_BINARY_DIR}/asset"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/asset"
        COMMENT "Copying assets to build and runtime directories"
)

# 构建时触发资源文件复制
add_executable(encrypt_test encrypt_test.cpp)
target_link_libraries(encrypt_test PUBLIC study-openssl)
add_dependencies(encrypt_test copy_assets)

add_executable(decrypt_test decrypt_test.cpp)
target_link_libraries(decrypt_test PUBLIC study-openssl)
add_dependencies(decrypt_test copy_assets)
