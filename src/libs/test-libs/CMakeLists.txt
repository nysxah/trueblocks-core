# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# folder project
project (test-libs)

# messaging
# message (WARNING  "*** Entering libs folder ***")

# Compile the libraries in this order
add_subdirectory(utillib)
add_subdirectory(etherlib)
add_subdirectory(acctlib)