

UPDATE pg_selta_version
   SET num=(case WHEN (length(substring(version(),'PostgreSQL (([0-9]).([0-9]).([0-9][0-9]))'))>0) THEN
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
	ver = version()
WHERE ver like '%PostgreSQL%';

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