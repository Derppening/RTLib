#[[

This file is part of RTLib.

Copyright (C) 2017 waicool20 <waicool20@gmail.com>
Copyright (C) 2018 Derppening <david.18.19.21@gmail.com>

RTLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RTLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with RTLib.  If not, see <http://www.gnu.org/licenses/>.

]]

# ------------- DOCS ---------------

message("----------Doxygen Information-----------")
find_package(Doxygen)

set(DOXYGEN_WARNINGS YES)
set(DOXYGEN_WARN_NO_PARAMDOC YES)
set(DOXYGEN_WARN_IF_DOC_ERROR YES)
set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN NO)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_HAVE_DOT YES)
set(DOXYGEN_CALL_GRAPH YES)
set(DOXYGEN_CALLER_GRAPH YES)
set(DOXYGEN_ENABLE_PREPROCESSING NO)

set(DOXYGEN_FULL_PATH_NAMES YES)
set(DOXYGEN_STRIP_FROM_PATH "${CMAKE_SOURCE_DIR}")
set(DOXYGEN_EXCLUDE_PATTERNS "${CMAKE_SOURCE_DIR}/libopencm3" "README.md")
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/doxygen")

doxygen_add_docs(
        doxygen
        ${PROJECT_SOURCE_DIR}
)
