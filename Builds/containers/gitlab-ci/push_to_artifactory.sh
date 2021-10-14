#!/usr/bin/env sh
set -ex
action=$1
filter=$2

. ./Builds/containers/gitlab-ci/get_component.sh

apk add curl jq coreutils util-linux
TOPDIR=$(pwd)

# DPKG

cd $TOPDIR
cd build/dpkg/packages
CURLARGS="-sk -X${action} -urippled:${ARTIFACTORY_DEPLOY_KEY_XRPLD}"
XRPLD_PKG=$(ls xrpld_*.deb)
XRPLD_DEV_PKG=$(ls xrpld-dev_*.deb)
XRPLD_DBG_PKG=$(ls xrpld-dbgsym_*.deb)
# TODO - where to upload src tgz?
XRPLD_SRC=$(ls xrpld_*.orig.tar.gz)
DEB_MATRIX=";deb.component=${COMPONENT};deb.architecture=amd64"
for dist in stretch buster xenial bionic disco focal ; do
    DEB_MATRIX="${DEB_MATRIX};deb.distribution=${dist}"
done
echo "{ \"debs\": {" > "${TOPDIR}/files.info"
for deb in ${XRPLD_PKG} ${XRPLD_DEV_PKG} ${XRPLD_DBG_PKG} ; do
    # first item doesn't get a comma separator
    if [ $deb != $XRPLD_PKG ] ; then
        echo "," >> "${TOPDIR}/files.info"
    fi
    echo "\"${deb}\"": | tee -a "${TOPDIR}/files.info"
    ca="${CURLARGS}"
    if [ "${action}" = "PUT" ] ; then
        url="https://${ARTIFACTORY_HOST}/artifactory/${DEB_REPO}/pool/${COMPONENT}/${deb}${DEB_MATRIX}"
        ca="${ca} -T${deb}"
    elif [ "${action}" = "GET" ] ; then
        url="https://${ARTIFACTORY_HOST}/artifactory/api/storage/${DEB_REPO}/pool/${COMPONENT}/${deb}"
    fi
    echo "file info request url --> ${url}"
    eval "curl ${ca} \"${url}\"" | jq -M "${filter}" | tee -a "${TOPDIR}/files.info"
done
echo "}," >> "${TOPDIR}/files.info"

# RPM

cd $TOPDIR
cd build/rpm/packages
XRPLD_PKG=$(ls xrpld-[0-9]*.x86_64.rpm)
XRPLD_DEV_PKG=$(ls xrpld-devel*.rpm)
XRPLD_DBG_PKG=$(ls xrpld-debuginfo*.rpm)
# TODO - where to upload src rpm ?
XRPLD_SRC=$(ls xrpld-[0-9]*.src.rpm)
echo "\"rpms\": {" >> "${TOPDIR}/files.info"
for rpm in ${XRPLD_PKG} ${XRPLD_DEV_PKG} ${XRPLD_DBG_PKG} ; do
    # first item doesn't get a comma separator
    if [ $rpm != $XRPLD_PKG ] ; then
        echo "," >> "${TOPDIR}/files.info"
    fi
    echo "\"${rpm}\"": | tee -a "${TOPDIR}/files.info"
    ca="${CURLARGS}"
    if [ "${action}" = "PUT" ] ; then
        url="https://${ARTIFACTORY_HOST}/artifactory/${RPM_REPO}/${COMPONENT}/"
        ca="${ca} -T${rpm}"
    elif [ "${action}" = "GET" ] ; then
        url="https://${ARTIFACTORY_HOST}/artifactory/api/storage/${RPM_REPO}/${COMPONENT}/${rpm}"
    fi
    echo "file info request url --> ${url}"
    eval "curl ${ca} \"${url}\"" | jq -M "${filter}" | tee -a "${TOPDIR}/files.info"
done
echo "}}" >> "${TOPDIR}/files.info"
jq '.' "${TOPDIR}/files.info" > "${TOPDIR}/files.info.tmp"
mv "${TOPDIR}/files.info.tmp" "${TOPDIR}/files.info"

if [ ! -z "${SLACK_NOTIFY_URL}" ] && [ "${action}" = "GET" ] ; then
    # extract files.info content to variable and sanitize so it can
    # be interpolated into a slack text field below
    finfo=$(cat ${TOPDIR}/files.info | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/\\n/g' | sed -E 's/"/\\"/g')
    # try posting file info to slack.
    # can add channel field to payload if the
    # default channel is incorrect. Get rid of
    # newlines in payload json since slack doesn't accept them
    CONTENT=$(tr -d '[\n]' <<JSON
       payload={
         "username": "GitlabCI",
         "text": "The package build for branch \`${CI_COMMIT_REF_NAME}\` is complete. File hashes are: \`\`\`${finfo}\`\`\`",
         "icon_emoji": ":package:"}
JSON
)
    curl ${SLACK_NOTIFY_URL} --data-urlencode "${CONTENT}"
fi

