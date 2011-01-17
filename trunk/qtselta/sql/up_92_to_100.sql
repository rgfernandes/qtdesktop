-- Update SELTA@Etersoft v.1.0 Beta 3 to SELTA@Etersoft v.1.0.0 (92 to 100)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver mvarchar);
insert into pg_selta_version values (100, 'SELTA@Etersoft v.1.0.0');

CREATE OR REPLACE FUNCTION DROP_INDEX(index_name varchar, table_name varchar) RETURNS INTEGER AS
$$
declare
is_con int;
begin
SELECT count(*) from pg_constraint where conname=index_name INTO is_con;
IF is_con>0 THEN
	execute 'ALTER TABLE '||table_name||' DROP CONSTRAINT '||index_name;
ELSE
	execute 'DROP INDEX '||index_name;
END IF;
DELETE FROM pg_indexes_name WHERE innername=to_mchar(index_name);
return 0;
end
$$ language plpgsql;

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
