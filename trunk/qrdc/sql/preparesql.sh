#!/bin/sh
# convert human-readable sql scripts into Qt-compatible (1 sql string per line)default_src.sql
./src2sql.sh scheme_src.sql scheme.sql
./src2sql.sh default_src.sql default.sql
