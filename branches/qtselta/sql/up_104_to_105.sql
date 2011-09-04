-- Update SELTA@Etersoft v.1.0.4 to SELTA@Etersoft v.1.0.5 (104 to 105)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver text);
insert into pg_selta_version values (105, 'SELTA@Etersoft v.1.0.5');
insert into pg_selta_version 
SELECT
	(case WHEN (length(substring(version(),'PostgreSQL (([0-9]).([0-9]).([0-9][0-9]))'))>0) THEN
		(substring(substring(version(),'PostgreSQL (([0-9]).([0-9]).([0-9][0-9]))'),1,1) ||
		substring(substring(version(),'PostgreSQL (([0-9]).([0-9]).([0-9][0-9]))'),3,1) ||
		substring(substring(version(),'PostgreSQL (([0-9]).([0-9]).([0-9][0-9]))'),5,2))
	WHEN (length(substring(version(),'PostgreSQL (([0-9]).([0-9]).([0-9]))'))>0) THEN
		(substring(substring(version(),'PostgreSQL (([0-9]).([0-9]).([0-9]))'),1,1) ||
		substring(substring(version(),'PostgreSQL (([0-9]).([0-9]).([0-9]))'),3,1) ||
		0||
		substring(substring(version(),'PostgreSQL (([0-9]).([0-9]).([0-9]))'),5,1))
	ELSE 
		(substring(substring(version(),'PostgreSQL (([0-9]).([0-9]))'),1,1) ||
		substring(substring(version(),'PostgreSQL (([0-9]).([0-9]))'),3,1) ||
		0||
		0)
	END) :: numeric, 
			version();

-- Declare new right and left functions:
CREATE OR REPLACE FUNCTION left(str mvarchar, num int) RETURNS mvarchar AS
$$
declare
tmp mvarchar;
begin
tmp := substr(str, 1, num);
return tmp;
end
$$ language plpgsql IMMUTABLE;

CREATE OR REPLACE FUNCTION right(str mchar, num int) RETURNS mchar AS
$$
declare
tmp mchar;
begin
tmp := substr(str, length(str)-num+1, 255);
return tmp;
end
$$ language plpgsql IMMUTABLE;

CREATE OR REPLACE FUNCTION left(str mchar, num int) RETURNS mchar AS
$$
declare
tmp mchar;
begin
tmp := substr(str, 1, num);
return tmp;
end
$$ language plpgsql IMMUTABLE;

-- Remove all procedure from 1C
select drop_function(name) from pg_func_names;
