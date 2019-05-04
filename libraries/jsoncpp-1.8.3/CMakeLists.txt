cmake_minimum_required (VERSION 2.6)
project (KRAKENAPI)

add_definitions("-std=c++11")


#-------------------------------------------------------------------------------
# Add library jsoncpp
#-------------------------------------------------------------------------------
add_library (jsoncpp STATIC src/jsoncpp.cpp)
