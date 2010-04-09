-- Show all tables:
SELECT datname FROM pg_database WHERE NOT datistemplate ORDER BY datname;
