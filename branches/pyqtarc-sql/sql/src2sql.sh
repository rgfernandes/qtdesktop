#!/bin/sh
# (c) Valily Makarov <drmoriarty.0@gmail.com>
# script to convert human-readable sql script into  sql-per-line
tr -d '\n' < data_src.sql | sed "s/);/);\n/g" | sed "s/END;/END;\n/g" | sed "s/\t/ /g" > data.sql
