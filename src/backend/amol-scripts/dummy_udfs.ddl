DROP FUNCTION expensive(int4, int4, int4);
DROP FUNCTION delay(int4, int4);
DROP FUNCTION setseed(int4);

CREATE FUNCTION expensive(int4, int4, int4) RETURNS bool AS 'dummy_udfs' LANGUAGE 'c';

CREATE FUNCTION setseed(int4) RETURNS void AS 'dummy_udfs' LANGUAGE 'c';

CREATE FUNCTION delay(int4, int4) RETURNS int4 AS 'dummy_udfs' LANGUAGE 'c';
