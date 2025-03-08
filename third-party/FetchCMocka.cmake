# Credit to OlivierLDff on GitHub for creating this

include(FetchContent)

# Declare our target. We want the lastest stable version, not the master.
# Also specify GIT_SHALLOW to avoid cloning branch we don't care about
FetchContent_Declare(
  cmocka
  GIT_REPOSITORY https://git.cryptomilk.org/projects/cmocka.git
  GIT_TAG        cmocka-1.1.5
  GIT_SHALLOW    1
)

# We want to link to cmocka (non static), so we need to set this option before calling the FetchContent_MakeAvailable
# We also don't care about example and tests
set(WITH_STATIC_LIB OFF CACHE BOOL "CMocka: Build with a static library" FORCE)
set(WITH_CMOCKERY_SUPPORT OFF CACHE BOOL "CMocka: Install a cmockery header" FORCE)
set(WITH_EXAMPLES OFF CACHE BOOL "CMocka: Build examples" FORCE)
set(UNIT_TESTING OFF CACHE BOOL "CMocka: Build with unit testing" FORCE)
set(PICKY_DEVELOPER OFF CACHE BOOL "CMocka: Build with picky developer flags" FORCE)

# Download cmocka, and execute its cmakelists.txt
FetchContent_MakeAvailable(cmocka)

