include_guard(GLOBAL)

# Set default global project variables
set(INKNIT_COMPANY_NAME "mntone")
set(INKNIT_PRODUCT_NAME "inknit")
set(INKNIT_LEGAL_COPYRIGHT "Copyright (C) 2025 mntone")
set(INKNIT_CMAKE_VERSION 0.8.0)

# Add common module directories to default search path
list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake"
	)
