cmake_minimum_required(VERSION 3.10)

# set the project name
project(mtfind)

# add the executable
add_executable(mtfind mtfind.cpp channel.cpp)

enable_testing()

function(do_test name pattern)
  add_test(NAME ${name} COMMAND ./test.sh ${pattern} out/${name})
endfunction(do_test)

do_test(simple "my")
do_test(wildcard "?ad")
do_test(space " ")
do_test(not_found "ttt")
do_test(tail "??ce")  # match at the end
do_test(long " ????????????????????????????????????????? ")
