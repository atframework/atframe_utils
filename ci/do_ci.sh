#!/bin/bash

cd "$(cd "$(dirname $0)" && pwd)/..";

set -ex ;

if [[ "$1" == "coverage" ]]; then
  if [[ "x$USE_SSL" == "xmbedtls" ]]; then
    vcpkg install --triplet=$VCPKG_TARGET_TRIPLET mbedtls ;
    CRYPTO_OPTIONS="-DATFRAMEWORK_CMAKE_TOOLSET_THIRD_PARTY_CRYPTO_USE_MBEDTLS=ON" ;
  else
    vcpkg install --triplet=$VCPKG_TARGET_TRIPLET openssl ;
    CRYPTO_OPTIONS="-DATFRAMEWORK_CMAKE_TOOLSET_THIRD_PARTY_CRYPTO_USE_OPENSSL=ON" ;
  fi
  vcpkg install --triplet=$VCPKG_TARGET_TRIPLET fmt ;
  bash cmake_dev.sh -lus -b Debug -r build_jobs_coverage -c $USE_CC -- $CRYPTO_OPTIONS "-DCMAKE_C_FLAGS=$GCOV_FLAGS" "-DCMAKE_CXX_FLAGS=$GCOV_FLAGS" "-DCMAKE_EXE_LINKER_FLAGS=$GCOV_FLAGS" -DCMAKE_TOOLCHAIN_FILE=$VCPKG_INSTALLATION_ROOT/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=$VCPKG_TARGET_TRIPLET ;
  cd build_jobs_coverage ;
  cmake --build . ;
  ctest . -V ;
elif [[ "$1" == "ssl.openssl" ]]; then
  CRYPTO_OPTIONS="-DATFRAMEWORK_CMAKE_TOOLSET_THIRD_PARTY_CRYPTO_USE_OPENSSL=ON" ;
  vcpkg install --triplet=$VCPKG_TARGET_TRIPLET fmt openssl ;
  bash cmake_dev.sh -lus -b Debug -r build_jobs_ci -c $USE_CC -- $CRYPTO_OPTIONS -DCMAKE_TOOLCHAIN_FILE=$VCPKG_INSTALLATION_ROOT/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=$VCPKG_TARGET_TRIPLET ;
  cd build_jobs_ci ;
  cmake --build . -j ;
  ctest . -V ;
elif [[ "$1" == "ssl.mbedtls" ]]; then
  CRYPTO_OPTIONS="-DATFRAMEWORK_CMAKE_TOOLSET_THIRD_PARTY_CRYPTO_USE_MBEDTLS=ON=ON" ;
  vcpkg install --triplet=$VCPKG_TARGET_TRIPLET fmt mbedtls ;
  bash cmake_dev.sh -lus -b Debug -r build_jobs_ci -c $USE_CC -- $CRYPTO_OPTIONS -DCMAKE_TOOLCHAIN_FILE=$VCPKG_INSTALLATION_ROOT/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=$VCPKG_TARGET_TRIPLET ;
  cd build_jobs_ci ;
  cmake --build . -j ;
  ctest . -V ;
elif [[ "$1" == "gcc.legacy.test" ]]; then
  bash cmake_dev.sh -lus -b Debug -r build_jobs_ci -c $USE_CC ;
  cd build_jobs_ci ;
  cmake --build . -j ;
  ctest . -V ;
elif [[ "$1" == "msys2.mingw.test" ]]; then
  pacman -S --needed --noconfirm mingw-w64-x86_64-cmake git mingw-w64-x86_64-git-lfs m4 curl wget tar autoconf automake mingw-w64-x86_64-toolchain mingw-w64-x86_64-libtool python || true ;
  git config --global http.sslBackend openssl ;
  mkdir -p build_jobs_ci ;
  cd build_jobs_ci ;
  cmake .. -G 'MinGW Makefiles' "-DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS" -DPROJECT_ENABLE_UNITTEST=ON -DPROJECT_ENABLE_SAMPLE=ON -DPROJECT_ENABLE_TOOLS=ON ;
  cmake --build . -j ;
  ctest . -V ;
fi