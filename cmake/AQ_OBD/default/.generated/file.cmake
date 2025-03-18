# The following variables contains the files used by the different stages of the build process.
set(AQ_OBD_default_default_XC16_FILE_TYPE_assemble
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../flash.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/where_was_i.s")
set(AQ_OBD_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${AQ_OBD_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE C)
set(AQ_OBD_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../at.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../commands.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/adc1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/clock.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/interrupt_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/mcc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/pin_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/reset.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/system.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/tmr1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/traps.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/uart1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../nvmpic.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../pps.c")
set(AQ_OBD_default_default_XC16_FILE_TYPE_link)

# The (internal) path to the resulting build image.
set(AQ_OBD_default_internal_image_name "${CMAKE_CURRENT_SOURCE_DIR}/../../../_build/AQ_OBD/default/default.elf")

# The name of the resulting image, including namespace for configuration.
set(AQ_OBD_default_image_name "AQ_OBD_default_default.elf")

# The name of the image, excluding the namespace for configuration.
set(AQ_OBD_default_original_image_name "default.elf")

# The output directory of the final image.
set(AQ_OBD_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/AQ_OBD")
