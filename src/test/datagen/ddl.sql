drop table R;
drop table S;

create table S(t timestamp TIMESTAMP, a int, b int, c int);
create table R(t timestamp TIMESTAMP, a int, b int, c int, d int);


copy S from '/home/sailesh/work/pgsql/S.data' USING DELIMITERS ',';
copy R from '/home/sailesh/work/pgsql/R.data' USING DELIMITERS ',';

DROP FUNCTION max(timestamp,timestamp);
CREATE FUNCTION max(timestamp,timestamp) RETURNS timestamp
AS 'SELECT CASE WHEN $1 > $2 THEN $1 ELSE $2 END;' LANGUAGE 'sql';

