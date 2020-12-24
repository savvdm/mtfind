cmake_minimum_required(VERSION 3.10)

# set the project name
project(mtfind)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(mtfind mtfind.cpp)
target_link_libraries(mtfind PRIVATE Threads::Threads)

enable_testing()

function(do_test name pattern)
  add_test(NAME ${name} COMMAND ./test.sh ${pattern} out/${name})
endfunction(do_test)

do_test(simple "my")
do_test(wildcard "?ad")
do_test(space " ")
do_test(all "?")
do_test(not_found "ttt")
do_test(tail "??ce")  # match at the end
do_test(long " ????????????????????????????????????????? ")
