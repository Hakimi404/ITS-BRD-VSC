# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/TI_Labor/GSy/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard"
  "C:/TI_Labor/GSy/ITS-BRD-VSC/Programs/Aufgabe2/tmp/1"
  "C:/TI_Labor/GSy/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard"
  "C:/TI_Labor/GSy/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/tmp"
  "C:/TI_Labor/GSy/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src/Aufgabe2.Debug+ITSboard-stamp"
  "C:/TI_Labor/GSy/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src"
  "C:/TI_Labor/GSy/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src/Aufgabe2.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/TI_Labor/GSy/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src/Aufgabe2.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/TI_Labor/GSy/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src/Aufgabe2.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
