-- Update SELTA@Etersoft v.1.0.6 to SELTA@Etersoft v.1.0.7 (105 to 106)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver text);
insert into pg_selta_version values (107, 'SELTA@Etersoft v.1.0.7');
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


-- Функции для создания/удаления курсоров пересоздания с удалением
--Создание курсора, если уже такой есть, удалим старый и пересоздадим
CREATE OR REPLACE FUNCTION createcurifexists(namecur name, textcur text)
  RETURNS int AS
$BODY$
	BEGIN
	EXECUTE 'DECLARE '||namecur||' SCROLL CURSOR WITH HOLD FOR '||textcur;
	RETURN 1;
	EXCEPTION 
		WHEN duplicate_cursor THEN
			EXECUTE 'CLOSE '||namecur;
			RETURN createcurifexists(namecur, textcur);
		
	END;
$BODY$
  LANGUAGE 'plpgsql';

--Закрытие курсора, если такого нет, ничего не делается
CREATE OR REPLACE FUNCTION closecurifexists(namecur name)
  RETURNS int AS
$BODY$
	BEGIN
	EXECUTE 'CLOSE '||namecur;
	RETURN 1;
	EXCEPTION WHEN	invalid_cursor_name
		THEN 
		RETURN 0;
	END;
$BODY$
  LANGUAGE 'plpgsql';

--Пересоздание курсора с перемещением вперед
CREATE OR REPLACE FUNCTION createcurifexistswithmoveforward(namecur name, textcur text, nmove integer)
  RETURNS integer AS
$BODY$
DECLARE
  retval integer;
BEGIN
	retval:=1;
	BEGIN
		EXECUTE 'CLOSE '||namecur;
	EXCEPTION 
		WHEN	invalid_cursor_name 
			THEN
			retval:=0;
	END;
	EXECUTE 'DECLARE '||namecur||' SCROLL CURSOR WITH HOLD FOR '||textcur;
	EXECUTE 'MOVE FORWARD '||nmove||' IN '||namecur;
	RETURN retval;
END;
		
$BODY$
  LANGUAGE 'plpgsql';


--Пересоздание курсора с перемещением сзади
CREATE OR REPLACE FUNCTION createcurifexistswithmovebackward(namecur name, textcur text, nmove integer)
  RETURNS integer AS

$BODY$
DECLARE
  retval integer;
BEGIN
	retval:=1;
	BEGIN
		EXECUTE 'CLOSE '||namecur;
	EXCEPTION 
		WHEN	invalid_cursor_name 
			THEN
			retval:=0;
	END;
	EXECUTE 'DECLARE '||namecur||' SCROLL CURSOR WITH HOLD FOR '||textcur;
	EXECUTE	'MOVE LAST IN '||namecur||';MOVE BACKWARD '|| nmove ||' IN '|| namecur||';';
	RETURN retval;
END;
		
$BODY$
  LANGUAGE 'plpgsql';


-- Remove all procedure from 1C
select drop_function(name) from pg_func_names;
