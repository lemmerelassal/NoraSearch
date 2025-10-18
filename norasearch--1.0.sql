CREATE FUNCTION norasearch(text, text, integer)
RETURNS int[][]
AS 'MODULE_PATHNAME', 'norasearch'
LANGUAGE C STRICT;
