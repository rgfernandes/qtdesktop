SET search_path = public;

BEGIN;

-- I/O functions

CREATE FUNCTION mchartypmod_in(cstring[])
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchartypmod_out(int4)
RETURNS cstring
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_in(cstring)
RETURNS mchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_out(mchar)
RETURNS cstring
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_send(mchar)
RETURNS bytea
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_recv(internal)
RETURNS mchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE TYPE mchar (
    INTERNALLENGTH = -1,
	INPUT = mchar_in,
	OUTPUT = mchar_out,
	TYPMOD_IN       = mchartypmod_in,
	TYPMOD_OUT      = mchartypmod_out,
	RECEIVE	= mchar_recv,
	SEND = mchar_send,
	STORAGE = extended
);

CREATE FUNCTION mchar(mchar, integer, boolean)
RETURNS mchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE CAST (mchar as mchar)
WITH FUNCTION mchar(mchar, integer, boolean) as IMPLICIT;

CREATE FUNCTION mvarchar_in(cstring)
RETURNS mvarchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_out(mvarchar)
RETURNS cstring
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_send(mvarchar)
RETURNS bytea
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_recv(internal)
RETURNS mvarchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE TYPE mvarchar (
    INTERNALLENGTH = -1,
	INPUT = mvarchar_in,
	OUTPUT = mvarchar_out,
	TYPMOD_IN       = mchartypmod_in,
	TYPMOD_OUT      = mchartypmod_out,
	RECEIVE	= mvarchar_recv,
	SEND = mvarchar_send,
	STORAGE = extended
);

CREATE FUNCTION mvarchar(mvarchar, integer, boolean)
RETURNS mvarchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE CAST (mvarchar as mvarchar)
WITH FUNCTION mvarchar(mvarchar, integer, boolean) as IMPLICIT;

--Operations and functions

CREATE FUNCTION length(mchar)
RETURNS int4
AS '$libdir/mchar', 'mchar_length'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION upper(mchar)
RETURNS mchar
AS '$libdir/mchar', 'mchar_upper'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION lower(mchar)
RETURNS mchar
AS '$libdir/mchar', 'mchar_lower'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_hash(mchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_concat(mchar, mchar)
RETURNS mchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE OPERATOR || (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	= 	mchar_concat
);

CREATE FUNCTION mchar_like(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_notlike(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE OPERATOR ~~ (
	LEFTARG     =   mchar,
	RIGHTARG	= 	mvarchar,
	PROCEDURE	= 	mchar_like,
	RESTRICT	= 	likesel,
	JOIN		= 	likejoinsel,
	NEGATOR		= 	'!~~'
);

CREATE OPERATOR !~~ (
	LEFTARG     =   mchar,
	RIGHTARG	= 	mvarchar,
	PROCEDURE	= 	mchar_notlike,
	RESTRICT	= 	nlikesel,
	JOIN		= 	nlikejoinsel,
	NEGATOR		= 	'~~'
);

CREATE FUNCTION mchar_regexeq(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_regexne(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE OPERATOR ~ (
	LEFTARG     =   mchar,
	RIGHTARG	= 	mchar,
	PROCEDURE	= 	mchar_regexeq,
	RESTRICT	= 	regexeqsel,
	JOIN		= 	regexeqjoinsel,
	NEGATOR		= 	'!~'
);

CREATE OPERATOR !~ (
	LEFTARG     =   mchar,
	RIGHTARG	= 	mchar,
	PROCEDURE	= 	mchar_regexne,
	RESTRICT	= 	regexnesel,
	JOIN		= 	regexnejoinsel,
	NEGATOR		= 	'~'
);

CREATE FUNCTION similar_escape(mchar, mchar)
RETURNS mchar
AS '$libdir/mchar', 'mchar_similar_escape'
LANGUAGE C IMMUTABLE;

CREATE FUNCTION length(mvarchar)
RETURNS int4
AS '$libdir/mchar', 'mvarchar_length'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION upper(mvarchar)
RETURNS mvarchar
AS '$libdir/mchar', 'mvarchar_upper'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION lower(mvarchar)
RETURNS mvarchar
AS '$libdir/mchar', 'mvarchar_lower'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_hash(mvarchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_concat(mvarchar, mvarchar)
RETURNS mvarchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE OPERATOR || (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	= 	mvarchar_concat
);

CREATE FUNCTION mvarchar_like(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION like_escape(mvarchar, mvarchar)
RETURNS mvarchar
AS '$libdir/mchar', 'mvarchar_like_escape'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_notlike(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE OPERATOR ~~ (
	LEFTARG     =   mvarchar,
	RIGHTARG	= 	mvarchar,
	PROCEDURE	= 	mvarchar_like,
	RESTRICT	= 	likesel,
	JOIN		= 	likejoinsel,
	NEGATOR		= 	'!~~'
);

CREATE OPERATOR !~~ (
	LEFTARG     =   mvarchar,
	RIGHTARG	= 	mvarchar,
	PROCEDURE	= 	mvarchar_notlike,
	RESTRICT	= 	nlikesel,
	JOIN		= 	nlikejoinsel,
	NEGATOR		= 	'~~'
);

CREATE FUNCTION mvarchar_regexeq(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_regexne(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE OPERATOR ~ (
	LEFTARG     =   mvarchar,
	RIGHTARG	= 	mvarchar,
	PROCEDURE	= 	mvarchar_regexeq,
	RESTRICT	= 	regexeqsel,
	JOIN		= 	regexeqjoinsel,
	NEGATOR		= 	'!~'
);

CREATE OPERATOR !~ (
	LEFTARG     =   mvarchar,
	RIGHTARG	= 	mvarchar,
	PROCEDURE	= 	mvarchar_regexne,
	RESTRICT	= 	regexnesel,
	JOIN		= 	regexnejoinsel,
	NEGATOR		= 	'~'
);

CREATE FUNCTION similar_escape(mvarchar, mvarchar)
RETURNS mvarchar
AS '$libdir/mchar', 'mvarchar_similar_escape'
LANGUAGE C IMMUTABLE;

CREATE FUNCTION substr (mchar, int4)
RETURNS mchar
AS '$libdir/mchar', 'mchar_substring_no_len'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION substr (mchar, int4, int4)
RETURNS mchar
AS '$libdir/mchar', 'mchar_substring'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION substr (mvarchar, int4)
RETURNS mvarchar
AS '$libdir/mchar', 'mvarchar_substring_no_len'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION substr (mvarchar, int4, int4)
RETURNS mvarchar
AS '$libdir/mchar', 'mvarchar_substring'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

-- Comparing
--    MCHAR

CREATE FUNCTION mchar_icase_cmp(mchar, mchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_icase_eq(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_icase_ne(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_icase_lt(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_icase_le(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_icase_gt(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_icase_ge(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;


CREATE OPERATOR < (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_icase_lt,
	COMMUTATOR	= 	'>',
	NEGATOR		= 	'>=',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR > (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_icase_gt,
	COMMUTATOR	= 	'<',
	NEGATOR		= 	'<=',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR <= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_icase_le,
	COMMUTATOR	= 	'>=',
	NEGATOR		= 	'>',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR >= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_icase_ge,
	COMMUTATOR	= 	'<=',
	NEGATOR		= 	'<',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR = (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_icase_eq,
	COMMUTATOR	= 	'=',
	NEGATOR		= 	'<>',
	RESTRICT	= 	eqsel,
	JOIN		= 	eqjoinsel,
	SORT1 		= 	'<',
	SORT2 		= 	'<',
	HASHES
);

CREATE OPERATOR <> (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_icase_ne,
	COMMUTATOR	= 	'<>',
	NEGATOR		= 	'=',
	RESTRICT	= 	neqsel,
	JOIN		= 	neqjoinsel
);

CREATE FUNCTION mchar_case_cmp(mchar, mchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_case_eq(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_case_ne(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_case_lt(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_case_le(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_case_gt(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_case_ge(mchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;


CREATE OPERATOR &< (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_case_lt,
	COMMUTATOR	= 	'&>',
	NEGATOR		= 	'&>=',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR &> (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_case_gt,
	COMMUTATOR	= 	'&<',
	NEGATOR		= 	'&<=',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR &<= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_case_le,
	COMMUTATOR	= 	'&>=',
	NEGATOR		= 	'&>',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR &>= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_case_ge,
	COMMUTATOR	= 	'&<=',
	NEGATOR		= 	'&<',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR &= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_case_eq,
	COMMUTATOR	= 	'&=',
	NEGATOR		= 	'&<>',
	RESTRICT	= 	eqsel,
	JOIN		= 	eqjoinsel,
	SORT1 		= 	'&<',
	SORT2 		= 	'&<'
);

CREATE OPERATOR &<> (
	LEFTARG		=	mchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mchar_case_ne,
	COMMUTATOR	= 	'&<>',
	NEGATOR		= 	'&=',
	RESTRICT	= 	neqsel,
	JOIN		= 	neqjoinsel
);

--MVARCHAR

CREATE FUNCTION mvarchar_icase_cmp(mvarchar, mvarchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_icase_eq(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_icase_ne(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_icase_lt(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_icase_le(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_icase_gt(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_icase_ge(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;


CREATE OPERATOR < (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_icase_lt,
	COMMUTATOR	= 	'>',
	NEGATOR		= 	'>=',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR > (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_icase_gt,
	COMMUTATOR	= 	'<',
	NEGATOR		= 	'<=',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR <= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_icase_le,
	COMMUTATOR	= 	'>=',
	NEGATOR		= 	'>',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR >= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_icase_ge,
	COMMUTATOR	= 	'<=',
	NEGATOR		= 	'<',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR = (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_icase_eq,
	COMMUTATOR	= 	'=',
	NEGATOR		= 	'<>',
	RESTRICT	= 	eqsel,
	JOIN		= 	eqjoinsel,
	SORT1 		= 	'<',
	SORT2 		= 	'<',
	HASHES
);

CREATE OPERATOR <> (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_icase_ne,
	COMMUTATOR	= 	'<>',
	NEGATOR		= 	'=',
	RESTRICT	= 	neqsel,
	JOIN		= 	neqjoinsel
);

CREATE FUNCTION mvarchar_case_cmp(mvarchar, mvarchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_case_eq(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_case_ne(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_case_lt(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_case_le(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_case_gt(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mvarchar_case_ge(mvarchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;


CREATE OPERATOR &< (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_case_lt,
	COMMUTATOR	= 	'&>',
	NEGATOR		= 	'&>=',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR &> (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_case_gt,
	COMMUTATOR	= 	'&<',
	NEGATOR		= 	'&<=',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR &<= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_case_le,
	COMMUTATOR	= 	'&>=',
	NEGATOR		= 	'&>',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR &>= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_case_ge,
	COMMUTATOR	= 	'&<=',
	NEGATOR		= 	'&<',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR &= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_case_eq,
	COMMUTATOR	= 	'&=',
	NEGATOR		= 	'&<>',
	RESTRICT	= 	eqsel,
	JOIN		= 	eqjoinsel,
	SORT1 		= 	'&<',
	SORT2 		= 	'&<'
);

CREATE OPERATOR &<> (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mvarchar_case_ne,
	COMMUTATOR	= 	'&<>',
	NEGATOR		= 	'&=',
	RESTRICT	= 	neqsel,
	JOIN		= 	neqjoinsel
);

--    MCHAR <> MVARCHAR

CREATE FUNCTION mc_mv_icase_cmp(mchar, mvarchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_icase_eq(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_icase_ne(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_icase_lt(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_icase_le(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_icase_gt(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_icase_ge(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;


CREATE OPERATOR < (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_icase_lt,
	COMMUTATOR	= 	'>',
	NEGATOR		= 	'>=',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR > (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_icase_gt,
	COMMUTATOR	= 	'<',
	NEGATOR		= 	'<=',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR <= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_icase_le,
	COMMUTATOR	= 	'>=',
	NEGATOR		= 	'>',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR >= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_icase_ge,
	COMMUTATOR	= 	'<=',
	NEGATOR		= 	'<',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR = (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_icase_eq,
	COMMUTATOR	= 	'=',
	NEGATOR		= 	'<>',
	RESTRICT	= 	eqsel,
	JOIN		= 	eqjoinsel,
	SORT1 		= 	'<',
	SORT2 		= 	'<'
);

CREATE OPERATOR <> (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_icase_ne,
	COMMUTATOR	= 	'<>',
	NEGATOR		= 	'=',
	RESTRICT	= 	neqsel,
	JOIN		= 	neqjoinsel
);

CREATE FUNCTION mc_mv_case_cmp(mchar, mvarchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_case_eq(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_case_ne(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_case_lt(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_case_le(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_case_gt(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mc_mv_case_ge(mchar, mvarchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;


CREATE OPERATOR &< (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_case_lt,
	COMMUTATOR	= 	'&>',
	NEGATOR		= 	'&>=',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR &> (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_case_gt,
	COMMUTATOR	= 	'&<',
	NEGATOR		= 	'&<=',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR &<= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_case_le,
	COMMUTATOR	= 	'&>=',
	NEGATOR		= 	'&>',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR &>= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_case_ge,
	COMMUTATOR	= 	'&<=',
	NEGATOR		= 	'&<',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR &= (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_case_eq,
	COMMUTATOR	= 	'&=',
	NEGATOR		= 	'&<>',
	RESTRICT	= 	eqsel,
	JOIN		= 	eqjoinsel,
	SORT1 		= 	'&<',
	SORT2 		= 	'&<'
);

CREATE OPERATOR &<> (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	=	mc_mv_case_ne,
	COMMUTATOR	= 	'&<>',
	NEGATOR		= 	'&=',
	RESTRICT	= 	neqsel,
	JOIN		= 	neqjoinsel
);

--    MVARCHAR <> MCHAR

CREATE FUNCTION mv_mc_icase_cmp(mvarchar, mchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_icase_eq(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_icase_ne(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_icase_lt(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_icase_le(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_icase_gt(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_icase_ge(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;


CREATE OPERATOR < (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_icase_lt,
	COMMUTATOR	= 	'>',
	NEGATOR		= 	'>=',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR > (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_icase_gt,
	COMMUTATOR	= 	'<',
	NEGATOR		= 	'<=',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR <= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_icase_le,
	COMMUTATOR	= 	'>=',
	NEGATOR		= 	'>',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR >= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_icase_ge,
	COMMUTATOR	= 	'<=',
	NEGATOR		= 	'<',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR = (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_icase_eq,
	COMMUTATOR	= 	'=',
	NEGATOR		= 	'<>',
	RESTRICT	= 	eqsel,
	JOIN		= 	eqjoinsel,
	SORT1 		= 	'<',
	SORT2 		= 	'<'
);

CREATE OPERATOR <> (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_icase_ne,
	COMMUTATOR	= 	'<>',
	NEGATOR		= 	'=',
	RESTRICT	= 	neqsel,
	JOIN		= 	neqjoinsel
);

CREATE FUNCTION mv_mc_case_cmp(mvarchar, mchar)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_case_eq(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_case_ne(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_case_lt(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_case_le(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_case_gt(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mv_mc_case_ge(mvarchar, mchar)
RETURNS bool
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;


CREATE OPERATOR &< (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_case_lt,
	COMMUTATOR	= 	'&>',
	NEGATOR		= 	'&>=',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR &> (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_case_gt,
	COMMUTATOR	= 	'&<',
	NEGATOR		= 	'&<=',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR &<= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_case_le,
	COMMUTATOR	= 	'&>=',
	NEGATOR		= 	'&>',
	RESTRICT	= 	scalarltsel,
	JOIN		= 	scalarltjoinsel
);

CREATE OPERATOR &>= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_case_ge,
	COMMUTATOR	= 	'&<=',
	NEGATOR		= 	'&<',
	RESTRICT	= 	scalargtsel,
	JOIN		= 	scalargtjoinsel
);

CREATE OPERATOR &= (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_case_eq,
	COMMUTATOR	= 	'&=',
	NEGATOR		= 	'&<>',
	RESTRICT	= 	eqsel,
	JOIN		= 	eqjoinsel,
	SORT1 		= 	'&<',
	SORT2 		= 	'&<'
);

CREATE OPERATOR &<> (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	=	mv_mc_case_ne,
	COMMUTATOR	= 	'&<>',
	NEGATOR		= 	'&=',
	RESTRICT	= 	neqsel,
	JOIN		= 	neqjoinsel
);

-- MCHAR - VARCHAR operations

CREATE FUNCTION mchar_mvarchar_concat(mchar, mvarchar)
RETURNS mvarchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE OPERATOR || (
	LEFTARG		=	mchar,
	RIGHTARG	=	mvarchar,
	PROCEDURE	= 	mchar_mvarchar_concat
);

CREATE FUNCTION mvarchar_mchar_concat(mvarchar, mchar)
RETURNS mvarchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE OPERATOR || (
	LEFTARG		=	mvarchar,
	RIGHTARG	=	mchar,
	PROCEDURE	= 	mvarchar_mchar_concat
);

CREATE FUNCTION mvarchar_mchar(mvarchar, integer, boolean)
RETURNS mchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE CAST (mvarchar as mchar)
WITH FUNCTION mvarchar_mchar(mvarchar, integer, boolean) as IMPLICIT;

CREATE FUNCTION mchar_mvarchar(mchar, integer, boolean)
RETURNS mvarchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE CAST (mchar as mvarchar)
WITH FUNCTION mchar_mvarchar(mchar, integer, boolean) as IMPLICIT;

-- Aggregates

CREATE FUNCTION mchar_larger(mchar, mchar)
RETURNS mchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE AGGREGATE max (
	BASETYPE	= 	mchar,
	SFUNC 		= 	mchar_larger,
	STYPE		= 	mchar,
	SORTOP		= 	'>'
);

CREATE FUNCTION mchar_smaller(mchar, mchar)
RETURNS mchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE AGGREGATE min (
	BASETYPE	= 	mchar,
	SFUNC 		= 	mchar_smaller,
	STYPE		= 	mchar,
	SORTOP		= 	'<'
);

CREATE FUNCTION mvarchar_larger(mvarchar, mvarchar)
RETURNS mvarchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE AGGREGATE max (
	BASETYPE	= 	mvarchar,
	SFUNC 		= 	mvarchar_larger,
	STYPE		= 	mvarchar,
	SORTOP		= 	'>'
);

CREATE FUNCTION mvarchar_smaller(mvarchar, mvarchar)
RETURNS mvarchar
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE AGGREGATE min (
	BASETYPE	= 	mvarchar,
	SFUNC 		= 	mvarchar_smaller,
	STYPE		= 	mvarchar,
	SORTOP		= 	'<'
);

-- B-tree support

CREATE OPERATOR CLASS mchar_icase_ops
DEFAULT FOR TYPE mchar USING btree AS
        OPERATOR        1       <  ,
		OPERATOR        2       <= ,
		OPERATOR        3       =  ,
		OPERATOR        4       >= ,
		OPERATOR        5       >  ,
		FUNCTION        1       mchar_icase_cmp(mchar, mchar),
        OPERATOR        1       <  (mchar, mvarchar),
		OPERATOR        2       <= (mchar, mvarchar),
		OPERATOR        3       =  (mchar, mvarchar),
		OPERATOR        4       >= (mchar, mvarchar),
		OPERATOR        5       >  (mchar, mvarchar),
		FUNCTION        1       mc_mv_icase_cmp(mchar, mvarchar);

CREATE OPERATOR CLASS mchar_case_ops
FOR TYPE mchar USING btree AS
        OPERATOR        1       &<  ,
               OPERATOR        2       &<= ,
               OPERATOR        3       &=  ,
               OPERATOR        4       &>= ,
               OPERATOR        5       &>  ,
               FUNCTION        1       mchar_case_cmp(mchar, mchar),
        OPERATOR        1       &<  (mchar, mvarchar),
               OPERATOR        2       &<= (mchar, mvarchar),
               OPERATOR        3       &=  (mchar, mvarchar),
               OPERATOR        4       &>= (mchar, mvarchar),
               OPERATOR        5       &>  (mchar, mvarchar),
               FUNCTION        1       mc_mv_case_cmp(mchar, mvarchar);

CREATE OPERATOR CLASS mchar_icase_ops
DEFAULT FOR TYPE mchar USING hash AS
		OPERATOR        1       =  ,
		FUNCTION        1       mchar_hash(mchar);

CREATE OPERATOR CLASS mvarchar_icase_ops
DEFAULT FOR TYPE mvarchar USING btree AS
        OPERATOR        1       <  ,
		OPERATOR        2       <= ,
		OPERATOR        3       =  ,
		OPERATOR        4       >= ,
		OPERATOR        5       >  ,
		FUNCTION        1       mvarchar_icase_cmp(mvarchar, mvarchar),
        OPERATOR        1       <  (mvarchar, mchar),
		OPERATOR        2       <= (mvarchar, mchar),
		OPERATOR        3       =  (mvarchar, mchar),
		OPERATOR        4       >= (mvarchar, mchar),
		OPERATOR        5       >  (mvarchar, mchar),
		FUNCTION        1       mv_mc_icase_cmp(mvarchar, mchar);

CREATE OPERATOR CLASS mvarchar_case_ops
FOR TYPE mvarchar USING btree AS
        OPERATOR        1       &<  ,
               OPERATOR        2       &<= ,
               OPERATOR        3       &=  ,
               OPERATOR        4       &>= ,
               OPERATOR        5       &>  ,
               FUNCTION        1       mvarchar_case_cmp(mvarchar, mvarchar),
        OPERATOR        1       &<  (mvarchar, mchar),
               OPERATOR        2       &<= (mvarchar, mchar),
               OPERATOR        3       &=  (mvarchar, mchar),
               OPERATOR        4       &>= (mvarchar, mchar),
               OPERATOR        5       &>  (mvarchar, mchar),
               FUNCTION        1       mv_mc_case_cmp(mvarchar, mchar);

CREATE OPERATOR CLASS mvarchar_icase_ops
DEFAULT FOR TYPE mvarchar USING hash AS
		OPERATOR        1       =  ,
		FUNCTION        1       mvarchar_hash(mvarchar);


-- Index support for LIKE

CREATE FUNCTION mchar_pattern_fixed_prefix(internal, internal, internal, internal)
RETURNS int4
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE FUNCTION mchar_greaterstring(internal)
RETURNS internal
AS '$libdir/mchar'
LANGUAGE C IMMUTABLE RETURNS NULL ON NULL INPUT;

CREATE OR REPLACE FUNCTION isfulleq_mchar(mchar, mchar)
RETURNS bool AS '$libdir/mchar'
LANGUAGE C CALLED ON NULL INPUT IMMUTABLE;

CREATE OR REPLACE FUNCTION fullhash_mchar(mchar)
RETURNS int4 AS '$libdir/mchar'
LANGUAGE C CALLED ON NULL INPUT IMMUTABLE;


CREATE OPERATOR == (
    LEFTARG     = mchar,
    RIGHTARG    = mchar,
    PROCEDURE   = isfulleq_mchar,
    COMMUTATOR  = '==',
    RESTRICT    = eqsel,
    JOIN        = eqjoinsel,
    HASHES
);

CREATE OPERATOR CLASS mchar_fill_ops
 FOR TYPE mchar USING hash AS
    OPERATOR    1   ==,
    FUNCTION    1   fullhash_mchar(mchar);

CREATE OR REPLACE FUNCTION isfulleq_mvarchar(mvarchar, mvarchar)
RETURNS bool AS '$libdir/mchar'
LANGUAGE C CALLED ON NULL INPUT IMMUTABLE;

CREATE OR REPLACE FUNCTION fullhash_mvarchar(mvarchar)
RETURNS int4 AS '$libdir/mchar'
LANGUAGE C CALLED ON NULL INPUT IMMUTABLE;


CREATE OPERATOR == (
    LEFTARG     = mvarchar,
    RIGHTARG    = mvarchar,
    PROCEDURE   = isfulleq_mvarchar,
    COMMUTATOR  = '==',
    RESTRICT    = eqsel,
    JOIN        = eqjoinsel,
    HASHES
);

CREATE OPERATOR CLASS mvarchar_fill_ops
 FOR TYPE mvarchar USING hash AS
    OPERATOR    1   ==,
    FUNCTION    1   fullhash_mvarchar(mvarchar);

COMMIT;
SET search_path = public;

