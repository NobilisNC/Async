add_library(async INTERFACE)

target_include_directories(async
	INTERFACE include
)

target_compile_features(
  async

  INTERFACE
    cxx_std_17
)