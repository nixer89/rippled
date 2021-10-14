#!/bin/sh

# Execute this script with a running Postgres server on the current host.
# It should work with the most generic installation of Postgres,
# and is necessary for xrpld to store data in Postgres.

# usage: sudo -u postgres ./initdb.sh
psql -c "CREATE USER xrpld"
psql -c "CREATE DATABASE xrpld WITH OWNER = xrpld"

