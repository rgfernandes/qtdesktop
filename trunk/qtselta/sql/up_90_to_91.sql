-- Update SELTA@Etersoft v.1.0 Beta to SELTA@Etersoft v.1.0 Beta 2 (90 to 91)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver mvarchar);
insert into pg_selta_version values (91, 'SELTA@Etersoft v.1.0 Beta 2');


create or replace function str(num numeric, len int) RETURNS mvarchar AS
$$
declare
tmp varchar;
tmp2 varchar;
len_sp int;
begin
tmp := num::varchar;
len_sp = len - char_length(tmp);
IF len>0 THEN
	tmp2 := repeat(' ', len_sp);
	return to_mchar(tmp2||tmp);
ELSE
	return to_mchar(tmp);
END IF;
end
$$ language plpgsql;
