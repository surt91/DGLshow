# :abacus: DGLshow

A simple ode solver (Runge Kutta 4) with a simple Qt based
visualization.

A very rudimentary GUI and a few sample ode (from simple mechanics) are
included.

To edit the initial values (like mass, position, velocity, ...) the
source has to be edited -- currently.

![Lorenz attractor](lorenz.png)

## :hammer_and_wrench: Build

Dependencies are Qt4 or Qt5. The program can be build with:

```bash
cd src; qmake; make
```

## :whale: Docker

If you are running Linux and have an X server installed (probably true, XWayland does also work), it is easiest to use the provided docker container:

```bash
docker-compose build
# this is needed to allow access to your X-server from within the Docker container
xhost +local:
docker-compose up
```
