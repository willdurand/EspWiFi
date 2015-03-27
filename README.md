EspWiFi
=======

[![Build
Status](https://travis-ci.org/willdurand/EspWiFi.svg?branch=master)](https://travis-ci.org/willdurand/EspWiFi)
[![Build
Status](https://webapi.biicode.com/v1/badges/willdurand/willdurand/EspWiFi/master)](https://www.biicode.com/willdurand/EspWiFi)

Arduino driver for the ESP8266 WiFi module.

Usage
-----

Please see [`EspWiFi.h`](https://github.com/willdurand/EspWiFi/blob/master/EspWiFi.h).
You can also find a few examples in the
[`examples/`](https://github.com/willdurand/EspWiFi/blob/master/examples) directory.

Tests
-----

You will find the tests for the library itself in the `test/` directory:

    cd test ; make && make test

You can run the `bin/test_examples.sh` script to build the bundled examples
(which somehow tests AVR compilation).

License
-------

EspWiFi is released under the MIT License. See the bundled LICENSE file for
details.
