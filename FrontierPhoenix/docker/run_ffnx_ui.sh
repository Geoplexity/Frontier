#!/usr/bin/env bash

docker container rm ffnx-ui-container
if [[ $? -ne 0 ]]; then
  echo "Error can be ignored, no container found to remove"
fi

pushd with_ui

vals=($(xauth list $DISPLAY | head -n 1))

XAUTH_ADD_ARG="${vals[1]} ${vals[2]}"

#echo "XAUTH_ADD_ARG = $XAUTH_ADD_ARG"

export XAUTH_ADD_ARG=$XAUTH_ADD_ARG

docker compose run --name ffnx-ui-container app
