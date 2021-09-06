option(ENABLE_ASAN OFF)
option(ENABLE_UBSAN OFF)

if (ENABLE_ASAN)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif ()

if (ENABLE_UBSAN)
  add_compile_options(-fsanitize=undefined)
  add_link_options(-fsanitize=undefined)
endif ()
