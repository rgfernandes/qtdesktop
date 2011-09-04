-- Update SELTA@Etersoft v.1.0 Beta 2 to SELTA@Etersoft v.1.0 Beta 3 (91 to 92)

drop table if exists pg_selta_version;
create table pg_selta_version(num int, ver mvarchar);
insert into pg_selta_version values (92, 'SELTA@Etersoft v.1.0 Beta 3');
