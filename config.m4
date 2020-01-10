PHP_ARG_WITH(clickhouse, for clickhouse support,
[  --with-clickhouse=DIR   Path to libclickhouse-cpp-lib])

PHP_ARG_ENABLE(overoptimization, whether to enable overoptimization,
[  --enable-overoptimization 
                          Enable miscellaneous weird optimizations default=no], no, no)

PHP_ARG_ENABLE(overoptimization, whether to enable overoptimization,
[  --enable-date-stringify 
                          Enable returning Date/DateTime fields as strings in local TZ default=no], no, no)

if test "$PHP_CLICKHOUSE" != "no"; then
  CXXFLAGS="-std=c++14"
  PHP_REQUIRE_CXX()
  AC_LANG_CPLUSPLUS

  PHP_SUBST(CLICKHOUSE_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++,,CLICKHOUSE_SHARED_LIBADD)
	
  if test "$PHP_OVEROPTIMIZATION" != "no"; then
	AC_DEFINE(OVEROPTIMIZATION,1,[Enable miscellaneous weird optimizations])
  fi

  if test "$PHP_DATE_STRINGIFY" != "no"; then
	AC_DEFINE(DATE_STRINGIFY,1,[Enable returning Date/DateTime fields as strings in local TZ])
  fi

  if test "$PHP_CLICKHOUSE" = "yes"; then
    PHP_CHECK_LIBRARY(clickhouse-cpp-lib, _ZN10clickhouse6Client4PingEv,
    [],[
      AC_MSG_ERROR([ClickHouse C++ library required.])
    ])

    PHP_ADD_LIBRARY(clickhouse-cpp-lib,,CLICKHOUSE_SHARED_LIBADD)
  else
    if test ! -f $PHP_CLICKHOUSE/include/client.h; then
      AC_MSG_ERROR(Unable to locate client.h)
    fi

    PHP_CHECK_LIBRARY(clickhouse-cpp-lib, _ZN10clickhouse6Client4PingEv,
    [],[
      AC_MSG_ERROR([ClickHouse C++ library required.])
    ],[
      -L$PHP_CLICKHOUSE/$PHP_LIBDIR
    ])

    PHP_ADD_LIBRARY_WITH_PATH(clickhouse-cpp-lib, $PHP_CLICKHOUSE/$PHP_LIBDIR, CLICKHOUSE_SHARED_LIBADD)
    PHP_ADD_INCLUDE($PHP_CLICKHOUSE/include)
  fi

	PHP_NEW_EXTENSION(clickhouse, bridge.cpp clickhouse.cpp, $ext_shared)
fi


