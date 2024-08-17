install(
    TARGETS wc_exe
    RUNTIME COMPONENT wc_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
