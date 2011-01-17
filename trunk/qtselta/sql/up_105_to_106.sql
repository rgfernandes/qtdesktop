-- Update SELTA@Etersoft v.1.0.5 to SELTA@Etersoft v.1.0.6 (105 to 106)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver text);
insert into pg_selta_version values (106, 'SELTA@Etersoft v.1.0.6');
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

CREATE OR REPLACE FUNCTION mc_mv_icase_eq(a mchar, b mvarchar )
  RETURNS boolean AS
  $BODY$
  begin
        return (a::mvarchar = b);
  end
  $BODY$
  LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION mv_mc_icase_eq(b mvarchar, a mchar )
  RETURNS boolean AS
  $BODY$
  begin
        return (a::mvarchar = b);
  end
  $BODY$
  LANGUAGE 'plpgsql';



CREATE OR REPLACE FUNCTION pg_create_trigger_for_update(trtablename name)
  RETURNS void AS
$BODY$
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
  		EXECUTE 'CREATE TRIGGER ' || trtablename || '_trigger_selta AFTER INSERT OR UPDATE OR DELETE ON ' || trtablename || ' FOR EACH STATEMENT  EXECUTE PROCEDURE fixtablechahges()' ;
		
		IF NOT EXISTS (SELECT 1 FROM pg_table_changes WHERE tabname = trtablename) THEN
			INSERT INTO pg_table_changes(tabname, numberchanges) VALUES (trtablename, 1);
		END IF;
  END IF;
END;
$BODY$
  LANGUAGE 'plpgsql';


CREATE OR REPLACE FUNCTION pg_create_delete_trigger_from_mask()
  RETURNS void AS
$BODY$
BEGIN
IF EXISTS (
	SELECT * 
	FROM pg_tables 
	WHERE tablename = '_1susers')
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

EXECUTE E'CREATE OR REPLACE FUNCTION pg_create_delete_trigger_from_mask()
  RETURNS void AS
\$BODY\$
  DECLARE rec record;
BEGIN
FOR rec IN SELECT tablename 
FROM pg_tables
WHERE schemaname = \'public\'
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
			WHERE tgname =  rec.tablename || \'_trigger_selta\'   ) 
		THEN
		EXECUTE \'CREATE TRIGGER \' || rec.tablename || \'_trigger_selta AFTER INSERT OR UPDATE OR DELETE ON \' || rec.tablename || \' FOR EACH STATEMENT  EXECUTE PROCEDURE fixtablechahges()\' ;
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
		EXECUTE \'DROP TRIGGER IF EXISTS \' || rec.tablename || \'_trigger_selta on \' || rec.tablename;
		DELETE FROM pg_table_changes WHERE tabname=rec.tablename;
	END IF;
END LOOP;
END;
\$BODY\$
  LANGUAGE \'plpgsql\';';

PERFORM pg_create_delete_trigger_from_mask();
END;
$BODY$
  LANGUAGE 'plpgsql' ;


SELECT pg_create_delete_trigger_from_mask();


-- Remove all procedure from 1C
-- select drop_function(name) from pg_func_names;
