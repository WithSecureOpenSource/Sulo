Sulo
====

Sulo is a dynamic instrumentation tool for Adobe Flash Player. It is built on [Pin](https://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool).

Supported Flash versions
------------------------

The following Flash Player builds are supported:

* 10.3.181.23 standalone debug
* 10.3.181.23 standalone non-debug
* 10.3.181.23 ActiveX
* 11.1.102.62 standadlone non-debug
* 11.1.102.62 ActiveX

You can add support for another Flash Player build by specifying some RVAs and offsets in `FlashPlayerConfigBuilder.cpp`.

Limitations
-----------

Sulo supports ActionScript3 method calls only - AVM1 is not (yet) supported.

Building
--------

The easiest way to build Sulo is to use the `sulo_vs2010.sln` solution file with Visual Studio 2010.

1. Download [Intel Pin kit for Visual Studio 2010](http://software.intel.com/sites/landingpage/pintool/downloads/pin-2.13-65163-msvc10-windows.zip)
2. Extract the ZIP
3. Clone Sulo to `pin-2.13-65163-msvc10-windows\source\tools\Sulo`
4. Open `sulo_vs2010.sln` and build the solution

Plugins
-------

Sulo comes with three plugins:

1. Call tracer - logs all ActionScript method calls, including arguments and return values
2. Flash dumper - dumps Flash objects loaded with Loader.loadBytes() to disk
3. SecureSWF - logs decrypted strings from secureSWF-protected files

Creating your own plugin is easy: just inherit your class from `ISuloPlugin`, implement the virtual methods, and add the object to `m_plugins` in `SuloPluginManager::init()`.

Instrumenting Flash Player with Sulo
------------------------------------

```
pin.exe -t source\tools\sulo\Debug\sulo.dll -- "C:\path\to\Adobe\Flash\Player.exe"
```

Command-line options
--------------------

| Option | Default | Plugin | Explanation |
|--------|---------|--------|--------------|
|fast | false | General | Enables faster analysis by disabling call trace logging |
|early_tracing | false | Call tracer | Start logging ActionScript method calls as early as possible (already before any calls from the actual Flash) |
|tracefile | "calltrace.txt" | Call tracer | Filename for storing the call trace |
|flash_dump_prefix | "dumped" | Flash dumper | Filename prefix for dumped Flash objects |
|secureswf | "" | SecureSWF | Name of the string secureSWF decryption method |


License
-------

[Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0)
