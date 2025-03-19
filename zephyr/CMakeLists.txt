if(CONFIG_OPENBMC_PHOSPHOR_LOGGING)
    zephyr_library()
    zephyr_library_sources(
        ${ZEPHYR_CURRENT_MODULE_DIR}/lib/elog.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/lib/lg2_logger.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/lib/sdjournal.cpp
        # ${ZEPHYR_CURRENT_MODULE_DIR}/elog_entry.cpp
        # ${ZEPHYR_CURRENT_MODULE_DIR}/elog_meta.cpp
        # ${ZEPHYR_CURRENT_MODULE_DIR}/elog_serialize.cpp
        # ${ZEPHYR_CURRENT_MODULE_DIR}/extensions.cpp
        # ${ZEPHYR_CURRENT_MODULE_DIR}/log_manager.cpp
        # ${ZEPHYR_CURRENT_MODULE_DIR}/util.cpp
    )
    zephyr_library_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/zephyr)

    zephyr_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/gen)
    zephyr_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/lib/include)
    # zephyr_library_compile_definitions(_GNU_SOURCE)

endif()
