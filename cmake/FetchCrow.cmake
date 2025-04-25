if(NOT TARGET Crow::Crow OR NOT TARGET Crow::mustache)
  FetchContent_Declare(
      crow
      GIT_REPOSITORY https://github.com/CrowCpp/crow.git
      GIT_TAG v1.0+1
  )
  FetchContent_MakeAvailable(crow)
endif()