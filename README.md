# PHP Extension for Apache Cassandra

A feature-rich and highly tunable PHP client library for
[Apache Cassandra] 3+ using Cassandra's binary protocol and
Cassandra Query Language v3.

This is a wrapper around the [DataStax C/C++ Driver for Apache Cassandra].


## Datastax Notes
__Note__: The driver extension is a wrapper around the 
          [DataStax C/C++ Driver for Apache Cassandra] and is a requirement for proper
          installation of the PHP extension binaries. Ensure these dependencies are met before proceeding.
__Note__: DataStax products do not support big-endian systems.


## Compatibility

This driver works with the Cassandra Query Language v3 (CQL3) and
Cassandra's native protocol. The current version works with:

* Apache Cassandra versions 3.0+
* PHP 8.0 8.1 8.2 8.3
  * 32-bit (x86) and 64-bit (x64)
  * Thread safe (TS) and non-thread safe (NTS)
* Compilers: GCC 4.1.2+, Clang 3.4+, and MSVC 2010/2012/2013/2015


## Installation

On Linux based systems you can install using the `install.sh` script, or the Makefile (`make`)
The install script will install the cpp-driver if it is not found, but the Makefile assumes the Cassandra CPP-Driver is installed.

Or you can build yourself

### CPP-Driver
```bash

  # Clone the Datastax CPP Driver
  git clone --recursive https://github.com/datastax/cpp-driver

  # Make the build directory and enter it
  mkdir cpp-driver/build && cd cpp-driver/build

  # CMake the project
  cmake -DCASS_CPP_STANDARD=17 \
        -DCASS_BUILD_STATIC=ON \
        -DCASS_BUILD_SHARED=ON \
        -DCASS_USE_TIMERFD=ON \
        -DCASS_USE_LIBSSH2=ON -DCASS_USE_ZLIB=ON \
        -DCMAKE_C_FLAGS="-fPIC" \
        -DCMAKE_CXX_FLAGS="-fPIC \
        -Wno-error=redundant-move" \
        -DCMAKE_BUILD_TYPE="RelWithInfo" \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DCASS_INSTALL_PKG_CONFIG=ON ..

  # Make and Install the driver
  make
  sudo make install
```

### Cassandra PHP Extension

```bash
# Enter the `ext` directory
cd ext

# PHPize the project
phpize

# Configure the extension
./configure --with-cassandra=/usr/local

# Make and install the php-cassandra-driver
make -j8
make install

```


## Quick Start

```php
<?php
$cluster   = Cassandra::cluster()                 // connects to localhost by default
                 ->build();
$keyspace  = 'system';
$session   = $cluster->connect($keyspace);        // create session, optionally scoped to a keyspace
$statement = new Cassandra\SimpleStatement(       // also supports prepared and batch statements
    'SELECT keyspace_name, columnfamily_name FROM schema_columnfamilies'
);
$future    = $session->executeAsync($statement);  // fully asynchronous and easy parallel execution
$result    = $future->get();                      // wait for the result, with an optional timeout

foreach ($result as $row) {                       // results and rows implement Iterator, Countable and ArrayAccess
    printf("The keyspace %s has a table called %s\n", $row['keyspace_name'], $row['columnfamily_name']);
}
```




## Contributing

Pull requests are welcome. 


## Copyright

&copy; DataStax, Inc.

Licensed under the Apache License, Version 2.0 (the “License”); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

