-- Update SELTA@Etersoft v.1.0.0 to SELTA@Etersoft v.1.0.1 (100 to 101)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver mvarchar);
insert into pg_selta_version values (101, 'SELTA@Etersoft v.1.0.1');

-- Alter functions VOLATILE to IMMUTABLE
ALTER FUNCTION strpos(str mvarchar, sim mchar) IMMUTABLE;
ALTER FUNCTION right(str mvarchar, num int) IMMUTABLE;
ALTER FUNCTION str(num numeric, len int) IMMUTABLE;
ALTER FUNCTION to_char(str mvarchar) IMMUTABLE;
ALTER FUNCTION to_mchar(str varchar) IMMUTABLE;
ALTER FUNCTION to_mchar(str mvarchar) IMMUTABLE;
ALTER FUNCTION to_mchar(val timestamp, mode varchar) IMMUTABLE;
ALTER FUNCTION to_mchar(num int) IMMUTABLE;
ALTER FUNCTION to_time(str mvarchar) IMMUTABLE;
ALTER FUNCTION to_time(str varchar) IMMUTABLE;
ALTER FUNCTION to_time(str timestamp) IMMUTABLE;

-- Remove all procedure from 1C
select drop_function(name) from pg_func_names;

-- Remove function from mchar.sql
drop function substr (mchar, int4, int4);