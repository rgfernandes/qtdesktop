# Rulez #

  * use standard directories where it's possible. There is no reason why use /opt or whatever. Look at LSB and EFS specs.
  * use correct file privileges in sources. Executable source files do not make sense.
  * your config/build tool should take care about location of libraries you provide - eg. Linux expects 64bit libs in PREFIX/lib64 - use LIB\_SUFFIX variable in cmake builds and try to find something for qmake too.
  * use correct desktop files (no extension in icon file names, no duplicate entries, correct categories...)
  * fix compiler warnings where it's possible (test it with -Wall)
  * preferred tarball format can be tar.bz2, tar.gz is acceptable too. Avoid using zip where it's possible.