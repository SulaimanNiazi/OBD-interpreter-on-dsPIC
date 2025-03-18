include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(AQ_OBD_default_library_list )

# Handle files with suffix s, for group default-XC16
if(AQ_OBD_default_default_XC16_FILE_TYPE_assemble)
add_library(AQ_OBD_default_default_XC16_assemble OBJECT ${AQ_OBD_default_default_XC16_FILE_TYPE_assemble})
    AQ_OBD_default_default_XC16_assemble_rule(AQ_OBD_default_default_XC16_assemble)
    list(APPEND AQ_OBD_default_library_list "$<TARGET_OBJECTS:AQ_OBD_default_default_XC16_assemble>")
endif()

# Handle files with suffix S, for group default-XC16
if(AQ_OBD_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(AQ_OBD_default_default_XC16_assemblePreproc OBJECT ${AQ_OBD_default_default_XC16_FILE_TYPE_assemblePreproc})
    AQ_OBD_default_default_XC16_assemblePreproc_rule(AQ_OBD_default_default_XC16_assemblePreproc)
    list(APPEND AQ_OBD_default_library_list "$<TARGET_OBJECTS:AQ_OBD_default_default_XC16_assemblePreproc>")
endif()

# Handle files with suffix c, for group default-XC16
if(AQ_OBD_default_default_XC16_FILE_TYPE_compile)
add_library(AQ_OBD_default_default_XC16_compile OBJECT ${AQ_OBD_default_default_XC16_FILE_TYPE_compile})
    AQ_OBD_default_default_XC16_compile_rule(AQ_OBD_default_default_XC16_compile)
    list(APPEND AQ_OBD_default_library_list "$<TARGET_OBJECTS:AQ_OBD_default_default_XC16_compile>")
endif()

add_executable(${AQ_OBD_default_image_name} ${AQ_OBD_default_library_list})

target_link_libraries(${AQ_OBD_default_image_name} PRIVATE ${AQ_OBD_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
AQ_OBD_default_link_rule(${AQ_OBD_default_image_name})

# Add bin2hex target for converting built file to a .hex file.
add_custom_target(AQ_OBD_default_Bin2Hex ALL
    ${MP_BIN2HEX} ${AQ_OBD_default_image_name})
add_dependencies(AQ_OBD_default_Bin2Hex ${AQ_OBD_default_image_name})

# Post build target to copy built file to the output directory.
add_custom_command(TARGET ${AQ_OBD_default_image_name} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory ${AQ_OBD_default_output_dir}
                    COMMAND ${CMAKE_COMMAND} -E copy ${AQ_OBD_default_image_name} ${AQ_OBD_default_output_dir}/${AQ_OBD_default_original_image_name}
                    BYPRODUCTS ${AQ_OBD_default_output_dir}/${AQ_OBD_default_original_image_name})
