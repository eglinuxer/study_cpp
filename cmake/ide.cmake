# ---------------------------------------------------------------------------------------
# IDE support for headers
# ---------------------------------------------------------------------------------------
set(STUDY_CPP_HEADERS_DIR "${CMAKE_CURRENT_LIST_DIR}/../include")

file(GLOB STUDY_CPP_TOP_HEADERS "${STUDY_CPP_HEADERS_DIR}/study_cpp/*.h")
set(STUDY_CPP_ALL_HEADERS ${STUDY_CPP_TOP_HEADERS})

source_group("Header Files\\study_cpp" FILES ${STUDY_CPP_TOP_HEADERS})
