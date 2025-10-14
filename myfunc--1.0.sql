CREATE FUNCTION add_one(integer) RETURNS integer
AS 'myfunc', 'add_one'
LANGUAGE C STRICT;
