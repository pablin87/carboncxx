
# CarbonCxx Library

This is a simple C++ library that helps you to log metrics to carbon graphite.

## Requeriments/Dependencies

- boost
- gcc compliant with c++11 (4.8 or superior)

## Compile

After you get the code, go into the project folder and type as follows:

$> mkdir build
$> cd build
$> cmake ..

If no dependencies erros occurs, then you are ready to compile:

$> make

Also, you can run the tests:

$> make test

## Usage

There is a simple example where you can find how to use the library in 
test/simple_example.cpp.

Tipically, you will instanciate one CarbonLogger object, and call the log_* 
functions to log stats. Also, you will need to call the run_dumping_thread() 
method from a separate thread.

### Thread Safety

Calling the method CarbonLogger -> log_hit, log_count and log_duration is thread
safe, so you can call them from different thread at 0 risk.

