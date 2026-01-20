# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "Z:/Dokumente/Shared_GS_2/ITS-BRD-VSC/Programs/Aufgabe4/tmp/Aufgabe4.Debug+ITSboard"
  "Z:/Dokumente/Shared_GS_2/ITS-BRD-VSC/Programs/Aufgabe4/tmp/1"
  "Z:/Dokumente/Shared_GS_2/ITS-BRD-VSC/Programs/Aufgabe4/tmp/Aufgabe4.Debug+ITSboard"
  "Z:/Dokumente/Shared_GS_2/ITS-BRD-VSC/Programs/Aufgabe4/tmp/Aufgabe4.Debug+ITSboard/tmp"
  "Z:/Dokumente/Shared_GS_2/ITS-BRD-VSC/Programs/Aufgabe4/tmp/Aufgabe4.Debug+ITSboard/src/Aufgabe4.Debug+ITSboard-stamp"
  "Z:/Dokumente/Shared_GS_2/ITS-BRD-VSC/Programs/Aufgabe4/tmp/Aufgabe4.Debug+ITSboard/src"
  "Z:/Dokumente/Shared_GS_2/ITS-BRD-VSC/Programs/Aufgabe4/tmp/Aufgabe4.Debug+ITSboard/src/Aufgabe4.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "Z:/Dokumente/Shared_GS_2/ITS-BRD-VSC/Programs/Aufgabe4/tmp/Aufgabe4.Debug+ITSboard/src/Aufgabe4.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "Z:/Dokumente/Shared_GS_2/ITS-BRD-VSC/Programs/Aufgabe4/tmp/Aufgabe4.Debug+ITSboard/src/Aufgabe4.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
