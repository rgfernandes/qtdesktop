-- Update SELTA@Etersoft v.1.0.3 to SELTA@Etersoft v.1.0.4 (103 to 104)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver mvarchar);
insert into pg_selta_version values (104, 'SELTA@Etersoft v.1.0.4');

-- add primary key constraints and indexes
ALTER TABLE pg_func_names ADD CONSTRAINT pk_pg_func_names_name PRIMARY KEY (name);
ALTER TABLE pg_indexes_name ADD CONSTRAINT pk_pg_indexes_name_innername PRIMARY KEY (innername);
CREATE INDEX pg_indexes_name_relname_index ON pg_indexes_name (relname);

DROP FUNCTION mchar_like(mchar, mchar) CASCADE;
CREATE OR REPLACE FUNCTION mchar_like(mchar, mvarchar)
 RETURNS bool
 AS '$libdir/mchar'
 LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;
 
DROP FUNCTION mchar_notlike(mchar, mchar) CASCADE;
CREATE OR REPLACE FUNCTION mchar_notlike(mchar, mvarchar)
 RETURNS bool
 AS '$libdir/mchar'
 LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;
 
DROP OPERATOR IF EXISTS ~~ (mchar, mchar);
CREATE OPERATOR ~~ (
 	LEFTARG     =   mchar,
 	RIGHTARG	= 	mvarchar,
 	PROCEDURE	= 	mchar_like,
 	RESTRICT	= 	likesel,
 	JOIN		= 	likejoinsel,
 	NEGATOR		= 	'!~~'
 	);
 
DROP OPERATOR IF EXISTS !~~ (mchar, mchar);
 CREATE OPERATOR !~~ (
 	LEFTARG     =   mchar,
 	RIGHTARG	= 	mvarchar,
 	PROCEDURE	= 	mchar_notlike,
 	RESTRICT	= 	nlikesel,
 	JOIN		= 	nlikejoinsel,
 	NEGATOR		= 	'~~'
);

DROP FUNCTION mchar_pattern_fixed_prefix(bool, internal, internal, internal, internal) CASCADE; 
CREATE OR REPLACE FUNCTION mchar_pattern_fixed_prefix(internal, internal, internal, internal)
 RETURNS int4
 AS '$libdir/mchar'
 LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;
 
DROP FUNCTION mchar_greaterstring(bool, internal) CASCADE; 
CREATE OR REPLACE FUNCTION mchar_greaterstring(internal)
 RETURNS internal
 AS '$libdir/mchar'
 LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;