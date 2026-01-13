# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/GS_myrepo/ITS-BRD-VSC/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard"
  "D:/GS_myrepo/ITS-BRD-VSC/Programs/Aufgabe5/tmp/1"
  "D:/GS_myrepo/ITS-BRD-VSC/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard"
  "D:/GS_myrepo/ITS-BRD-VSC/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/tmp"
  "D:/GS_myrepo/ITS-BRD-VSC/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src/Aufgabe5.Debug+ITSboard-stamp"
  "D:/GS_myrepo/ITS-BRD-VSC/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src"
  "D:/GS_myrepo/ITS-BRD-VSC/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src/Aufgabe5.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/GS_myrepo/ITS-BRD-VSC/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src/Aufgabe5.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/GS_myrepo/ITS-BRD-VSC/Programs/Aufgabe5/tmp/Aufgabe5.Debug+ITSboard/src/Aufgabe5.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
