
# CarbonCxx Library

This is a simple C++ library that helps you to log metrics to carbon graphite.

## Requeriments/Dependencies

- boost
- gcc compliant with c++11 (4.8 or superior)

## Compile

After you get the code, go into the project folder and type as follows:

```
$> mkdir build
$> cd build
$> cmake ..
```

If no dependencies erros occurs, then you are ready to compile:

```
$> make
```

Also, you can run the tests:

```
$> make test
```

## Installation

This will install the headers and the library in the default installation path.
Check cmake to install things in other places (CMAKE_INSTAL_PREFIX).

```
$> sudo make install
```

That was really hard, right ? :)


## Usage

There is a simple example where you can find how to use the library in 
test/simple_example.cpp.

Tipically, you will instanciate one CarbonLogger object, and call the log_* 
functions to log stats. Also, you will need to firt call the init() and then
 the run_dumping_thread() methods to make the library work.
 
To compile your probgram you will have to link against the library, so you wil 
need to use the -lcarboncxx flag. 
For example to compile the example program that came with the code, once you 
have installed the library you can do as follows:

```
$>g++ simple_example.cpp -std=c++11 -lboost_system -lcarboncxx
```

### Thread Safety

Calling the method CarbonLogger -> log_hit, log_count and log_duration is thread
safe, so you can call them from different threads at 0 risk.

