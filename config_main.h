#pragma once

// const char* ERRLOG_PERSIST_PATH = "/var/lib/phosphor-logging/errors";
// const char* EXTENSION_PERSIST_DIR = "/var/lib/phosphor-logging/extensions";
const char* ERRLOG_PERSIST_PATH = CONFIG_FS_ROOT_MNT "/var/lib/phosphor-logging/errors";
const char* EXTENSION_PERSIST_DIR = CONFIG_FS_ROOT_MNT "/var/lib/phosphor-logging/extensions";
// const char* ERRLOG_PERSIST_PATH = "/RAM:/var/lib/phosphor-logging/errors";
// const char* EXTENSION_PERSIST_DIR = "/RAM:/var/lib/phosphor-logging/extensions";

const bool IS_UNIT_TEST = false;
