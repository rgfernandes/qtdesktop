-- SELTA@Etersoft v.1.0.5

-- Удаляем функции
drop function fixtablechahges() CASCADE;
drop function pg_create_trigger_for_update (trtablename name)  CASCADE;
drop function pg_create_delete_trigger_from_mask() CASCADE;
drop function LoadTable (table_name text, file_name text) CASCADE;
drop function GetIndexRelName (innname varchar) CASCADE;
drop function exec_query (query text);
drop function DROP_FUNCTION(small_name text) CASCADE;
drop function DROP_INDEX(index_name varchar, table_name varchar) CASCADE;
drop function UPDATE_OR_QUERY (p_query_update text, p_query_if int,  p_query_then text) CASCADE;
drop function obj_rename(old_name varchar, new_name varchar) CASCADE;
drop function strpos(str mvarchar, sim mchar) CASCADE;
drop function str(num numeric, len int) CASCADE;
drop function right(str mvarchar, num int) CASCADE;
drop function right(str mchar, num int) CASCADE;
drop function left(str mvarchar, num int) CASCADE;
drop function left(str mchar, num int) CASCADE;

drop function to_char(str mvarchar) CASCADE;
drop function to_mchar(str varchar) CASCADE;
drop function to_mchar(val timestamp, mode varchar) CASCADE;
drop function to_mchar(str mvarchar) CASCADE;
drop function to_mchar(num int) CASCADE;
drop function to_time(str mvarchar) CASCADE;
drop function to_time(str varchar) CASCADE;
drop function to_time(str timestamp) CASCADE;
drop function mvarchar_to_numeric(str mvarchar) CASCADE;
drop function numeric_to_mvarchar(num numeric) CASCADE;



-- Удаляем типы данных
drop domain tinyint CASCADE;
drop domain ms_bit CASCADE;

-- Удаляем таблицы
drop table pg_func_names CASCADE;
drop table pg_indexes_name CASCADE;
drop table if exists pg_selta_version;
drop table pg_table_changes CASCADE;
drop table pg_trigger_table_mask CASCADE;