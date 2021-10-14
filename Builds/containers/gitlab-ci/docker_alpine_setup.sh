#!/usr/bin/env sh
set -ex
# used as a before/setup script for docker steps in gitlab-ci
# expects to be run in standard alpine/dind image
echo $(nproc)
docker login -u xrpld \
    -p ${ARTIFACTORY_DEPLOY_KEY_XRPLD} ${ARTIFACTORY_HUB}
apk add --update py-pip
apk add \
    bash util-linux coreutils binutils grep \
    make ninja cmake build-base gcc g++ abuild git \
    python3 python3-dev
pip3 install awscli
# list curdir contents to build log:
ls -la

