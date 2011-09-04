-- Update SELTA@Etersoft v.1.0.1 to SELTA@Etersoft v.1.0.2 (101 to 102)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver mvarchar);
insert into pg_selta_version values (102, 'SELTA@Etersoft v.1.0.2');


CREATE OR REPLACE FUNCTION mvarchar_to_numeric(str mvarchar) returns numeric AS
$$
declare
begin
return numeric_in(mvarchar_out(str), 0, 0);
end
$$ language plpgsql IMMUTABLE;

CREATE OR REPLACE FUNCTION numeric_to_mvarchar(num numeric) returns mvarchar AS
$$
declare
begin
return mvarchar_in(numeric_out(num));
end
$$ language plpgsql IMMUTABLE;

CREATE CAST (mvarchar AS numeric) WITH FUNCTION mvarchar_to_numeric(mvarchar);
CREATE CAST (numeric AS mvarchar) WITH FUNCTION numeric_to_mvarchar(numeric);
