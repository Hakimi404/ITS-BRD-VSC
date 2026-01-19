# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/TEMP/Downloads/GS_Hakimi/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard"
  "C:/Users/TEMP/Downloads/GS_Hakimi/Programs/Aufgabe5/tmp/1"
  "C:/Users/TEMP/Downloads/GS_Hakimi/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard"
  "C:/Users/TEMP/Downloads/GS_Hakimi/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/tmp"
  "C:/Users/TEMP/Downloads/GS_Hakimi/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src/Aufgabe5.Debug+ITSboard-stamp"
  "C:/Users/TEMP/Downloads/GS_Hakimi/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src"
  "C:/Users/TEMP/Downloads/GS_Hakimi/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src/Aufgabe5.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/TEMP/Downloads/GS_Hakimi/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src/Aufgabe5.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/TEMP/Downloads/GS_Hakimi/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src/Aufgabe5.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
