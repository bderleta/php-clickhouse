# php-clickhouse
Implementation of ClickHouse C++ client as a PHP 7+ extension. Does not cover data inserting functionality yet.

The main goal is to achieve better performance than pure-PHP clients, by using binary protocol, and better memory management. It has been tested
to stream millions of rows for processing, while utilizing less than 16 MB of process memory. It is also around 2-3x faster than streaming resultsets over 
ClickHouse's HTTP protocol. Any non-SELECT query may be still performed using Select/Execute calls.

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

## TODO

- Support for Date / DateTime fields using strftime
- Support for large numeric values non-natively supported by PHP (UInt64, Int128, Decimal) as strings
- Support for Array, Tuple and the rest of complex fields 

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