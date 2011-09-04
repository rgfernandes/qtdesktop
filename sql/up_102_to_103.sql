-- Update SELTA@Etersoft v.1.0.2 to SELTA@Etersoft v.1.0.3 (102 to 103)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver mvarchar);
insert into pg_selta_version values (103, 'SELTA@Etersoft v.1.0.3');

-- recreate drop function
CREATE OR REPLACE FUNCTION DROP_FUNCTION(small_name text) RETURNS VOID AS
$$
declare
all_name text;
begin
select into all_name full_name from pg_func_names where name=small_name LIMIT 1;
if all_name is null then
return;
end if;
execute('DROP FUNCTION IF EXISTS ' || all_name);
DELETE FROM pg_func_names WHERE name=small_name;
end
$$ language plpgsql;

-- Remove all procedure from 1C
select drop_function(name) from pg_func_names;
