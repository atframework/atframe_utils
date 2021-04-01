# atframe_utils
cxx utils code

|                           | [Linux+OSX(clang+gcc)][linux-link] | [Windows+MinGW(vc+gcc)][windows-link] |
|:-------------------------:|:----------------------------------:|:-------------------------------------:|
| Build & Unit Test         | ![linux-badge]                     | ![windows-badge]                      |
Compilers | linux-clang-10/11 <br /> linux-gcc-9 <br /> linux-gcc-10 <br /> ~~macOS-clang-12.0~~ <br /> | MSVC 14(Visual Studio 2015) <br /> MSVC 15(Visual Studio 2017) <br /> MSVC 16(Visual Studio 2019) <br /> Mingw32-gcc <br /> Mingw64-gcc |  

[![codecov badge]][coverage status] [![lgtm-badge]][lgtm-link]

[linux-badge]: https://github.com/atframework/atframe_utils/actions/workflows/main.yml/badge.svg "Github action build status"
[linux-link]:  https://github.com/atframework/atframe_utils/actions/workflows/main.yml "Github action build status"
[windows-badge]: https://ci.appveyor.com/api/projects/status/7e6q54xxdga6ov00?svg=true "AppVeyor build status"
[windows-link]:  https://ci.appveyor.com/project/owt5008137/atframe-utils/branch/master "AppVeyor build status"
[codecov badge]: https://codecov.io/gh/atframework/atlist-rs/branch/master/graph/badge.svg
[coverage status]: https://codecov.io/gh/atframework/atlist-rs
[lgtm-badge]: https://img.shields.io/lgtm/grade/cpp/g/atframework/atframe_utils.svg?logo=lgtm&logoWidth=18 "LGTM"
[lgtm-link]:  https://lgtm.com/projects/g/atframework/atframe_utils/context:cpp "LGTM"

# Usage:

+ require [cmake][cmake] 3.7.0 or upper
+ require gcc 4.4+/clang 3.4+/apple clang 6.0+/MSVC 14+

~~~~~~~~~~bash
# clone and make build directory
git clone --single-branch --depth=1 -b master https://github.com/atframework/atframe_utils.git
mkdir atframe_utils/build && cd atframe_utils/build

# run cmake
# cmake <atframe_utils dir> [options...]
cmake .. -DPROJECT_ENABLE_SAMPLE=YES -DPROJECT_ENABLE_UNITTEST=YES #  -DCMAKE_INSTALL_PREFIX=<install prefix>

# build
cmake --build . # using clang or gcc
# cmake --build . --config RelWithDebInfo # using MSVC

# run unit test
ctest . -V

# run unit test
cmake --build . --target INSTALL # using clang or gcc
# cmake --build . --config RelWithDebInfo --target INSTALL # using MSVC
~~~~~~~~~~

## Other options

| Option  | Description |
|---------|-------------|
| BUILD\_SHARED\_LIBS=YES\|NO | [default=NO] Build dynamic library |
| LIBUNWIND\_ENABLED=YES\|NO | [default=NO] Enable and using libunwind for callstack unwind |
| LOG\_WRAPPER\_ENABLE\_LUA\_SUPPORT=YES\|NO | [default=YES] Enable lua support for log system |
| LOG\_WRAPPER\_CHECK\_LUA=YES\|NO | [default=YES] Enable checking for lua support |
| LOG\_WRAPPER\_ENABLE\_STACKTRACE=YES\|NO | [default=YES] Enable stack trace for log system |
| ENABLE\_MIXEDINT\_MAGIC\_MASK=0-8 | [default=0] Set mixed int mask |
| CRYPTO\_DISABLED=YES\|NO | [default=NO] Disable crypto and DH/ECDH support |
| CRYPTO\_USE\_OPENSSL=YES\|NO | [default=NO] Using openssl for crypto and DH/ECDH support, and close auto detection |
| CRYPTO\_USE\_MBEDTLS=YES\|NO | [default=NO] Using mbedtls for crypto and DH/ECDH support, and close auto detection |

[cmake]: https://cmake.org/