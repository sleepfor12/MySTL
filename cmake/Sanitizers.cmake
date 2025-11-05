# Sanitizers configuration for MySTL
# Provides AddressSanitizer, UndefinedBehaviorSanitizer, etc.

option(MYSTL_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(MYSTL_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(MYSTL_ENABLE_MSAN "Enable MemorySanitizer" OFF)
option(MYSTL_ENABLE_TSAN "Enable ThreadSanitizer" OFF)

if(MYSTL_ENABLE_ASAN OR MYSTL_ENABLE_UBSAN OR MYSTL_ENABLE_MSAN OR MYSTL_ENABLE_TSAN)
  if(MSVC)
    message(WARNING "Sanitizers are not fully supported on MSVC")
  else()
    set(SANITIZER_FLAGS "")
    
    if(MYSTL_ENABLE_ASAN)
      list(APPEND SANITIZER_FLAGS "address")
    endif()
    
    if(MYSTL_ENABLE_UBSAN)
      list(APPEND SANITIZER_FLAGS "undefined")
    endif()
    
    if(MYSTL_ENABLE_MSAN)
      list(APPEND SANITIZER_FLAGS "memory")
    endif()
    
    if(MYSTL_ENABLE_TSAN)
      list(APPEND SANITIZER_FLAGS "thread")
    endif()
    
    if(SANITIZER_FLAGS)
      string(REPLACE ";" "," SANITIZER_FLAGS "${SANITIZER_FLAGS}")
      add_compile_options(-fsanitize=${SANITIZER_FLAGS})
      add_link_options(-fsanitize=${SANITIZER_FLAGS})
    endif()
  endif()
endif()

