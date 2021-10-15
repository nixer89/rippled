# Xrpld Docker Image

Xrpld has a continuous deployment pipeline that turns every git commit into a
docker image for quick testing and deployment.

To run the tip of the latest release via docker:

```$ docker run -P -v /srv/xrpld/ xrpl/xrpld:latest```

To run the tip of active development:

```$ docker run -P -v /srv/xrpld/ xrpl/xrpld:develop```

Where ```/srv/xrpld``` points to a directory containing a xrpld.cfg and
database files. By default, port 5005/tcp maps to the RPC port and 51235/udp to
the peer port.
