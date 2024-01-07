# Dev environment setup
To create a Docker image with the required dependencies etc.

```shell
# note, you need to have Docker installed (tested with 23.0.1 but will probably work with most versions)
pushd docker
./build_base_environment.sh
```

See `docker/README.md` for more info.

## IDE setup

- Using Docker: You can use the Docker image created in build_dev_env.sh . If using CLion the setup can be configured:
    - Settings -> Build Execution Deployment -> Toolchains -> Add Docker (using gdb)
    - When adding dependencies/changing compiler you need to run Tools -> Resync with remote hosts
- Locally: You'll need to set up the required dependencies yourself. I recommend checking the Dockerfile to see what you 
  need to do (Note that some paths will probably differ since container commands run as root).
  - After doing that you should be able to build normally.