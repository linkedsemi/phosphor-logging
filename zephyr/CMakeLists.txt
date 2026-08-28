if(CONFIG_OPENBMC_PHOSPHOR_LOGGING)
    zephyr_library_named(phosphor-logging)
    zephyr_library_sources(
        ${ZEPHYR_CURRENT_MODULE_DIR}/lib/elog.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/lib/lg2_logger.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/lib/sdjournal.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/elog_entry.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/elog_meta.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/elog_serialize.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/extensions.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/log_manager_main.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/log_manager.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/util.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/gen/elog-process-metadata.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/gen/elog-lookup.cpp
        ${ZEPHYR_CURRENT_MODULE_DIR}/gen/xyz/openbmc_project/Logging/Internal/Manager/server.cpp
    )
    zephyr_library_include_directories(
        ${ZEPHYR_CURRENT_MODULE_DIR}
        ${ZEPHYR_CURRENT_MODULE_DIR}/gen
        ${ZEPHYR_CURRENT_MODULE_DIR}/lib/include
        )

    zephyr_export_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/lib/include)

    zephyr_library_compile_options(-fexceptions)

    zephyr_library_link_libraries(basu)
    zephyr_library_link_libraries(boost)
    zephyr_library_link_libraries(sdbusplus)
    zephyr_library_link_libraries(sdeventplus)
    zephyr_library_link_libraries(phosphor-dbus-interfaces)
    zephyr_library_link_libraries(cereal)

endif()
