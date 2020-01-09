# php-clickhouse
Implementation of ClickHouse C++ client as a PHP 7+ extension

## Prerequisites

- PHP 7.3
- Requires [clickhouse-cpp-lib](https://github.com/ClickHouse/clickhouse-cpp/)

## Building

```
$ phpize
$ ./configure
$ make
# make install
```

Enable the extension in `php.ini` via `extension=clickhouse.so` and optionally restart your webserver/FPM.

## Usage

Extension exposes `ClickHouse` class.

```ClickHouse::__construct(
	?string $hostname = null, 
	?string $user = null, 
	?string $password = null, 
	?string $default_database = null, 
	?int $port = 9000, 
	bool $compression = true
)```

Creates a ClickHouse instance, immediately connecting to the machine.

```ClickHouse::select(
	string $query, 
	callable $onBlockRead
)```

Invokes a SELECT query, calling `$onBlockRead(array $rows)` on every received block of data.