# Dockerfiles for base and UI environment

## If you just want to build on the host

The dockerfiles under base_environment and with_ui can be used as a guide to
install the various dependencies needed for the library. I would advise running
cmake and fixing missing dependencies as they crop up rather than copying all
the steps from the Dockerfiles.

## Docker: If you just want to compile the library and do not need UI:

```shell
./build_base_environment.sh
```

## Docker: If you intend to run the UI inside docker

```shell
./build_base_environment.sh
./build_ui_environment.sh
./run_ffnx_ui.sh
```

Note: running UI applications inside Docker can be a bit tricky. At time of writing the
scripts used to launch the UI from Docker work on the following host. YMMV.

```shell
$ lsb_release  -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 22.04.3 LTS
Release:	22.04
Codename:	jammy
```
