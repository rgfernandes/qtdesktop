-- SELTA@Etersoft v.1.0.7

-- Version of Selta
drop table if exists pg_selta_version;
CREATE TABLE pg_selta_version(num int, ver text);
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

-- Создаем типы данных
CREATE DOMAIN tinyint AS smallint constraint con_byte check (value>=0 and value<= 255);
CREATE DOMAIN ms_bit AS smallint constraint con_byte check (value>=0 and value<= 1);

-- Создаем таблицы
CREATE TABLE pg_func_names (
name varchar,
full_name varchar,
CONSTRAINT pk_pg_func_names_name PRIMARY KEY (name)
);

CREATE TABLE pg_indexes_name (
relname mvarchar,
tabname mvarchar,
innername mvarchar,
CONSTRAINT pk_pg_indexes_name_innername PRIMARY KEY (innername)
);

CREATE INDEX pg_indexes_name_relname_index ON pg_indexes_name (relname);

--таблица, содержащая информацию о количестви изменений в таблице
CREATE TABLE pg_table_changes (
  tabname name NOT NULL,
  numberchanges integer NOT NULL,
  CONSTRAINT pk_pg_table_changes_tabname PRIMARY KEY (tabname)
);

CREATE UNIQUE INDEX pk_pg_pg_table_changes_tabname ON pg_table_changes(tabname);

--таблица с масками, по которым создаются триггеры

CREATE TABLE pg_trigger_table_mask (
  mask name,
  include boolean,
  equal boolean,
  CONSTRAINT pg_trigger_table_mask_mask_key UNIQUE (mask)
);







--------------------------------------------------------------------------------------------------------
-- Создаем все функции
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


-- Триггерная функия для подсчета изменений 
CREATE OR REPLACE FUNCTION fixtablechahges() RETURNS trigger AS 
$BODY$ 	
begin 
UPDATE pg_table_changes 
SET numberchanges = numberchanges + 1 
WHERE tabname = TG_RELNAME; 
return new; 
end	
$BODY$ 
LANGUAGE 'plpgsql';

-- функция, которая создает триггеры, которыt соответсвуют маске из таблицы pg_trigger_table_mask 

CREATE OR REPLACE FUNCTION pg_create_trigger_for_update (trtablename name)
  RETURNS void AS
$BODY$
BEGIN
IF trtablename = '_1susers'
THEN
	INSERT INTO pg_trigger_table_mask(
		    mask, include, equal)
	    VALUES ('sc%', true, false),('cj%', true, false),('_1sjourn', true, true),('1stoper', true, true),('_1soper', true, true),('_1scorent', true, true),('_1sentry', true, true),('_1saccs', true, true),('pg_%', false, false);
ELSE
	INSERT INTO pg_trigger_table_mask(
		     mask, include, equal)
	    VALUES ('%', true, false); 
	INSERT INTO pg_trigger_table_mask(
		    mask, include, equal)
	    VALUES ('pg_%', false, false);
END IF;

EXECUTE E'
CREATE OR REPLACE FUNCTION pg_create_trigger_for_update (trtablename name)
  RETURNS void AS
\$BODY\$
BEGIN
	IF EXISTS
		(SELECT 1 
		FROM pg_tables as ptmain
		WHERE 
		(
		(EXISTS
			(SELECT * 
			FROM pg_tables as pt INNER JOIN 
							(SELECT * 
							FROM pg_trigger_table_mask 
							WHERE include = true AND equal = true) as pttmtt 
							ON tablename = mask WHERE pt.tablename = ptmain.tablename)
		OR EXISTS
			(SELECT * 
			FROM pg_tables as pt INNER JOIN 
							(SELECT * 
							FROM pg_trigger_table_mask 
							WHERE include = true AND equal = false) as pttmtf 
							ON tablename LIKE mask WHERE pt.tablename = ptmain.tablename)
		
		AND NOT EXISTS
			(SELECT * 
			FROM pg_tables as pt INNER JOIN 
							(SELECT * 
							FROM pg_trigger_table_mask 
							WHERE include = false AND equal = true) as pttmft 
							ON tablename = mask WHERE pt.tablename = ptmain.tablename)
		AND NOT EXISTS
			(SELECT * 
			FROM pg_tables as pt INNER JOIN 
							(SELECT * 
							FROM pg_trigger_table_mask 
							WHERE include = false AND equal = false) as pttmff 
							ON tablename LIKE mask WHERE pt.tablename = ptmain.tablename))
		)
		AND ptmain.tablename = trtablename)
	THEN
  		EXECUTE \'CREATE TRIGGER \' || trtablename || \'_trigger_selta AFTER INSERT OR UPDATE OR DELETE ON \' || trtablename || \' FOR EACH STATEMENT  EXECUTE PROCEDURE fixtablechahges()\' ;
		
		IF NOT EXISTS (SELECT 1 FROM pg_table_changes WHERE tabname = trtablename) THEN
			INSERT INTO pg_table_changes(tabname, numberchanges) VALUES (trtablename, 1);
		END IF;
  END IF;
END;
\$BODY\$
  LANGUAGE \'plpgsql\'';

PERFORM pg_create_trigger_for_update(trtablename);
END;
$BODY$
  LANGUAGE 'plpgsql' ;


--Делает все триггеры, которые соответствуют маске, и удаляет существующие, которые не соответствует.

CREATE OR REPLACE FUNCTION pg_create_delete_trigger_from_mask()
  RETURNS void AS
$BODY$
  DECLARE rec record;
BEGIN
FOR rec IN SELECT tablename 
FROM pg_tables
WHERE schemaname = 'public'
LOOP
	IF EXISTS
		(SELECT * 
		FROM pg_tables as ptmain
		WHERE 
		(
		(EXISTS
			(SELECT * 
			FROM pg_tables as pt INNER JOIN 
							(SELECT * 
							FROM pg_trigger_table_mask 
							WHERE include = true AND equal = true) as pttmtt 
							ON tablename = mask WHERE pt.tablename = ptmain.tablename)
		OR EXISTS
			(SELECT * 
			FROM pg_tables as pt INNER JOIN 
							(SELECT * 
							FROM pg_trigger_table_mask 
							WHERE include = true AND equal = false) as pttmtf 
							ON tablename LIKE mask WHERE pt.tablename = ptmain.tablename)
		
		AND NOT EXISTS
			(SELECT * 
			FROM pg_tables as pt INNER JOIN 
							(SELECT * 
							FROM pg_trigger_table_mask 
							WHERE include = false AND equal = true) as pttmft 
							ON tablename = mask WHERE pt.tablename = ptmain.tablename)
		AND NOT EXISTS
			(SELECT * 
			FROM pg_tables as pt INNER JOIN 
							(SELECT * 
							FROM pg_trigger_table_mask 
							WHERE include = false AND equal = false) as pttmff 
							ON tablename LIKE mask WHERE pt.tablename = ptmain.tablename))
		)
		AND ptmain.tablename = rec.tablename)
	THEN
	
		IF NOT EXISTS 
			(SELECT * 
			FROM pg_trigger 
			WHERE tgname =  rec.tablename || '_trigger_selta'   ) 
		THEN
		EXECUTE 'CREATE TRIGGER ' || rec.tablename || '_trigger_selta AFTER INSERT OR UPDATE OR DELETE ON ' || rec.tablename || ' FOR EACH STATEMENT  EXECUTE PROCEDURE fixtablechahges()' ;
		END IF;
		IF NOT EXISTS 
			(SELECT * 
			FROM pg_table_changes 
			WHERE tabname = rec.tablename)
		THEN
				INSERT INTO pg_table_changes (
				tabname, numberchanges)
				VALUES (rec.tablename, 1);
		END IF;
	ELSE 
		EXECUTE 'DROP TRIGGER IF EXISTS ' || rec.tablename || '_trigger_selta on ' || rec.tablename;
		DELETE FROM pg_table_changes WHERE tabname=rec.tablename;
	END IF;
END LOOP;
END;
$BODY$
  LANGUAGE 'plpgsql';


--функции приводящие типы при сравнении. Стандартные 1с не работали в таблицах. появлялась ошибка ERROR: could not find member 1(416621,416621) of opfamily 416784
CREATE OR REPLACE FUNCTION mv_mc_icase_eq(b mvarchar, a mchar )
  RETURNS boolean AS
  $BODY$
  begin
        return (a::mvarchar = b);
  end
  $BODY$
  LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION mc_mv_icase_eq(a mchar, b mvarchar)
  RETURNS boolean AS
  $BODY$
  begin
        return (a::mvarchar = b);
  end
  $BODY$
  LANGUAGE 'plpgsql';

-- По внутреннему имени возвращает внешнее
CREATE OR REPLACE FUNCTION GetIndexRelName (innname varchar) returns varchar as 
$$ 
declare
name mvarchar;
begin
select relname from pg_indexes_name where innername=to_mchar(innname) into name;
if FOUND then
	return to_char(name);
else
	return NULL;
end if;
end
$$ language plpgsql;

-- Загружает таблицу из текстового файла. Старые данные удаляются!!! Так же обрабатывается автоинкрементное поле
CREATE OR REPLACE FUNCTION LoadTable (table_name text, file_name text) returns varchar as 
$$ 
declare
name varchar;
colname varchar;
max_col int = -1;
seq_name varchar;
begin
SELECT tablename FROM pg_tables WHERE tablename=lower(table_name) INTO name;
IF NOT FOUND THEN return 'Table '||table_name||' not found!'; END IF;
execute 'DELETE FROM '||table_name;
execute 'COPY '||table_name||' FROM '''||file_name||''' delimiter AS '';'' NULL AS '''' CSV';
-- Устанавливаем счетчики если они есть в таблицах
-- Ищем есть ли автоинкрементные поля
select attname,adsrc from pg_attribute,pg_attrdef where attrelid=(select relfilenode from pg_class where relname=name) 
	and attrelid=adrelid and adnum=attnum INTO colname,seq_name;
-- Если есть, определяем новое значение и устанавливаем его
if FOUND THEN
execute 'select max('||colname||') from '||name INTO max_col;
if max_col > 0 THEN
seq_name := substr(seq_name, 10);
seq_name := substr(seq_name, 1, strpos(seq_name, '''')-1);
PERFORM setval(seq_name, max_col);
END IF;
END IF;
return 'Load is OK';
end
$$ language plpgsql;


CREATE OR REPLACE FUNCTION exec_query (query text) returns int as 
$$ 
declare 
i int = 0;
begin
if substr(query, 1, 8)='SELECT 1' OR substr(query, 1, 8)='SELECT 0' then
	execute query into i;
else
	execute query;
end if;
return i;
end
$$ language plpgsql;


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



-- Для второго параметра указаны следующие обозначения
-- 0: @@ROWCOUNT = 0
-- 1: @@ROWCOUNT > 0
CREATE OR REPLACE FUNCTION UPDATE_OR_QUERY (p_query_update text, p_query_if int,  p_query_then text) returns void as 
$$ 
declare
i int;
begin
execute p_query_update;
get diagnostics i=row_count;
if p_query_if=0 then
if i=0 then 
execute(p_query_then);
end if;
elseif p_query_if=1 then
if i>0 then 
execute(p_query_then);
end if;
end if;
end
$$ language plpgsql;



CREATE OR REPLACE FUNCTION obj_rename(old_name varchar, new_name varchar) RETURNS VOID AS
$$
declare
type_obj char;
tmp_name mvarchar;
-- Для переименовывания индексов
ind REFCURSOR;
ind_name varchar;
tab_name varchar;
ind_rel_old_name varchar;
ind_inn_new_name varchar;
begin
-- Определеяем тип объекта - Таблица или Индекс
select innername from pg_indexes_name where relname=to_mchar(old_name) into tmp_name;
if found then
	select relkind from pg_class where relname=lower(old_name) or relname=lower(to_char(tmp_name)) into type_obj;
else
	select relkind from pg_class where relname=lower(old_name) into type_obj;
end if;
-- Если таблица...
if type_obj='r' OR type_obj='c' OR type_obj='t' THEN
execute('ALTER TABLE '||old_name||' RENAME TO '||new_name);
execute('ALTER TABLE _insert_'||old_name||' RENAME TO _insert_'||new_name);
OPEN ind FOR execute('SELECT innername, relname FROM pg_indexes_name WHERE tabname = '''||old_name||'''');
FETCH ind INTO ind_rel_old_name, ind_name;
WHILE FOUND LOOP
execute('ALTER INDEX '||ind_rel_old_name||' RENAME TO '||ind_name||'_'||new_name);
-- решаем багу с переименовыванием индексов, если они входят в ограничения таблиц constraint
execute('update pg_constraint SET conname = '''||lower(ind_name||'_'||new_name)||''' where conname = '''||lower(ind_rel_old_name)||'''');
update pg_indexes_name set tabname=to_mchar(new_name), innername=to_mchar(ind_name||'_'||new_name) where innername=to_mchar(ind_rel_old_name);
FETCH ind INTO ind_rel_old_name, ind_name;
END LOOP;
CLOSE ind;
end if;
-- Если индекс...
if type_obj='i' THEN
select innername, tabname from pg_indexes_name where relname=to_mchar(old_name) into ind_rel_old_name, tab_name;
ind_inn_new_name := new_name||'_'||tab_name;
execute('ALTER INDEX '||ind_rel_old_name||' RENAME TO '||ind_inn_new_name);
-- решаем багу с переименовыванием индексов, если они входят в ограничения таблиц constraint
execute('update pg_constraint SET conname = '''||lower(ind_inn_new_name)||''' where conname = '''||lower(ind_rel_old_name)||'''');
-- Изменяем значения таблицы имен индексов
update pg_indexes_name set relname=to_mchar(new_name), innername=to_mchar(ind_inn_new_name) where relname=to_mchar(old_name);
end if;
end
$$ language plpgsql;


--Функция удаления индексов. Проверяет, является ли индекс констреинтом
--и в зависимости от этого выполняет разные команды по удалению
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



CREATE OR REPLACE FUNCTION strpos(str mvarchar, sim mchar) RETURNS INTEGER AS
$$
declare
begin
return strpos((varcharin(mvarchar_out(str), 0, -1))::varchar, (varcharin(mvarchar_out(sim), 0, -1))::char);
end
$$ language plpgsql IMMUTABLE;


CREATE OR REPLACE FUNCTION right(str mvarchar, num int) RETURNS mvarchar AS
$$
declare
tmp mvarchar;
begin
tmp := substr(str, length(str)-num+1, 255);
return tmp;
end
$$ language plpgsql IMMUTABLE;

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

CREATE OR REPLACE FUNCTION str(num numeric, len int) RETURNS mvarchar AS
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
$$ language plpgsql IMMUTABLE;


CREATE OR REPLACE FUNCTION to_char(str mvarchar) returns varchar AS
$$
declare
begin
return varcharin(mvarchar_out(str), 0, -1);
end
$$ language plpgsql IMMUTABLE;


CREATE OR REPLACE FUNCTION to_mchar(str varchar) returns mvarchar AS
$$
declare
begin
return mvarchar_in(varcharout(str));
end
$$ language plpgsql IMMUTABLE;

CREATE OR REPLACE FUNCTION to_mchar(str mvarchar) returns mvarchar AS
$$
declare
begin
return str;
end
$$ language plpgsql IMMUTABLE;


CREATE OR REPLACE FUNCTION to_mchar(val timestamp, mode varchar) returns mvarchar AS
$$
declare
begin
return mvarchar_in(varcharout(to_char(val, mode)));
end
$$ language plpgsql IMMUTABLE;


CREATE OR REPLACE FUNCTION to_mchar(num int) returns mvarchar AS
$$
declare
begin
return mvarchar_in(int4out(num));
end
$$ language plpgsql IMMUTABLE;


CREATE OR REPLACE FUNCTION to_time(str mvarchar) returns timestamp AS
$$
declare
begin
return timestamp_in(mvarchar_out(str), 0, -1);
end
$$ language plpgsql IMMUTABLE;


CREATE OR REPLACE FUNCTION to_time(str varchar) returns timestamp AS
$$
declare
begin
return str::timestamp;
end
$$ language plpgsql IMMUTABLE;


CREATE OR REPLACE FUNCTION to_time(str timestamp) returns timestamp AS
$$
declare
begin
return str;
end
$$ language plpgsql IMMUTABLE;

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

--------------------------------------------------------------------------------------------------------
-- Create CAST

CREATE CAST (mvarchar AS numeric) WITH FUNCTION mvarchar_to_numeric(mvarchar);
CREATE CAST (numeric AS mvarchar) WITH FUNCTION numeric_to_mvarchar(numeric);

--------------------------------------------------------------------------------------------------------
-- Создаем все операторы

CREATE OPERATOR + (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	= 	mchar_concat
);

CREATE OPERATOR + (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	= 	mvarchar_concat
);

CREATE OPERATOR + (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	= 	mchar_mvarchar_concat
);

CREATE OPERATOR + (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	= 	mvarchar_mchar_concat
);

