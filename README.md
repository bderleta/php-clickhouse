# php-clickhouse
Implementation of ClickHouse C++ client as a PHP 7+ extension. Does not cover data inserting functionality yet.

The main goal is to achieve better performance than pure-PHP clients, by using binary protocol, and better memory management. 

Optional weird optimizations, like returning results as indexed array instead of associative ones may be enabled using `./configure --enable-overoptimization`. 
They provide some speedup, but at the cost of losing some functionality, so by default that option is disabled.

## Prerequisites

- PHP 7.3
- C++14 capable toolset
- [clickhouse-cpp-lib](https://github.com/ClickHouse/clickhouse-cpp/) built and installed

## Building

```
$ phpize
$ ./configure
$ make
# make install
```

Enable the extension in `php.ini` via `extension=clickhouse.so` and optionally restart your webserver/FPM.

## Quirks

### Support for large numeric values not supported natively by PHP (UInt64, Int128, Decimal) as strings

UInt64 and Int128 family (Decimal types) are converted to string using code of rather mediocre performance.
There are no sights of forecoming native support for such high / accurate numeric values in PHP.

### Date/DateTime support

By default, Date and DateTime values are returned as integer (UNIX timestamps). Optionally, you can enable returning them as a formatted string in local timezone,
using `./configure --enable-date-stringify`.

## To-do

- Support for Array, Tuple
- Support for IPv4, IPv6 and UUID types

## Usage

Extension exposes `ClickHouse` class.

```
ClickHouse::__construct(
	?string $hostname = null, 
	?string $user = null, 
	?string $password = null, 
	?string $default_database = null, 
	?int $port = 9000, 
	bool $compression = true
)
```

Creates a ClickHouse instance, immediately connecting to the machine.

```
ClickHouse::select(
	string $query, 
	callable $onBlockRead
) : int
```

Invokes a SELECT query, calling `$onBlockRead(array $rows)` on every received block of data. If callback returns `FALSE`, fetching stops and no further callbacks are made.
Returns total number of rows received.

```
ClickHouse::execute(
	string $query
) : bool
```

Invokes a query. Returns `TRUE` or throws exception on error.

```
ClickHouse::ping(
) : bool
```

Pings the server. Returns `TRUE` or throws exception on error.

```
ClickHouse::resetConnection(
) : bool
```

Resets the connection. Returns `TRUE` or throws exception on error.