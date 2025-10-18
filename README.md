Under `MSYS2 MINGW64`, navigate to this folder and run
`make`

Copy `norasearch.dll` to C:\Program Files\PostgreSQL\18\lib

Copy `norasearch.control` and `norasearch--1.0.sql` to C:\Program Files\PostgreSQL\18\share\extension

In PostgreSQL, open a query window (or use `psql`) and enter the following:

``
CREATE EXTENSION norasearch;
SELECT norasearch('abracadabra', 'abra', 2);
``

should return

{{0,4},{7,4}}