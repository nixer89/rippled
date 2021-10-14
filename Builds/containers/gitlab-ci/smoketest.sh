#!/usr/bin/env sh
set -ex
install_from=$1
use_private=${2:-0} # this option not currently needed by any CI scripts,
                    # reserved for possible future use
if [ "$use_private" -gt 0 ] ; then
    REPO_ROOT="https://xrpld:${ARTIFACTORY_DEPLOY_KEY_XRPLD}@${ARTIFACTORY_HOST}/artifactory"
else
    REPO_ROOT="${PUBLIC_REPO_ROOT}"
fi

. ./Builds/containers/gitlab-ci/get_component.sh

. /etc/os-release
case ${ID} in
    ubuntu|debian)
        pkgtype="dpkg"
        ;;
    fedora|centos|rhel|scientific)
        pkgtype="rpm"
        ;;
    *)
        echo "unrecognized distro!"
        exit 1
        ;;
esac

# this script provides info variables about pkg version
. build/${pkgtype}/packages/build_vars

if [ "${pkgtype}" = "dpkg" ] ; then
    # sometimes update fails and requires a cleanup
    updateWithRetry()
    {
        if ! apt-get -y update ; then
            rm -rvf /var/lib/apt/lists/*
            apt-get -y clean
            apt-get -y update
        fi
    }
    if [ "${install_from}" = "repo" ] ; then
        apt-get -y upgrade
        updateWithRetry
        apt-get -y install apt apt-transport-https ca-certificates coreutils util-linux wget gnupg
        wget -q -O - "${REPO_ROOT}/api/gpg/key/public" | apt-key add -
        echo "deb ${REPO_ROOT}/${DEB_REPO} ${DISTRO} ${COMPONENT}" >> /etc/apt/sources.list
        updateWithRetry
        # uncomment this next line if you want to see the available package versions
        # apt-cache policy xrpld
        apt-get -y install xrpld=${dpkg_full_version}
    elif [ "${install_from}" = "local" ] ; then
        # cached pkg install
        updateWithRetry
        apt-get -y install libprotobuf-dev libssl-dev
        rm -f build/dpkg/packages/xrpld-dbgsym*.*
        dpkg --no-debsig -i build/dpkg/packages/*.deb
    else
        echo "unrecognized pkg source!"
        exit 1
    fi
else
    yum -y update
    if [ "${install_from}" = "repo" ] ; then
        yum -y install yum-utils coreutils util-linux
        REPOFILE="/etc/yum.repos.d/artifactory.repo"
        echo "[Artifactory]" > ${REPOFILE}
        echo "name=Artifactory" >> ${REPOFILE}
        echo "baseurl=${REPO_ROOT}/${RPM_REPO}/${COMPONENT}/" >> ${REPOFILE}
        echo "enabled=1" >> ${REPOFILE}
        echo "gpgcheck=0" >> ${REPOFILE}
        echo "gpgkey=${REPO_ROOT}/${RPM_REPO}/${COMPONENT}/repodata/repomd.xml.key" >> ${REPOFILE}
        echo "repo_gpgcheck=1" >> ${REPOFILE}
        yum -y update
        # uncomment this next line if you want to see the available package versions
        # yum --showduplicates list xrpld
        yum -y install ${rpm_version_release}
    elif [ "${install_from}" = "local" ] ; then
        # cached pkg install
        yum install -y yum-utils openssl-static zlib-static
        rm -f build/rpm/packages/xrpld-debug*.rpm
        rm -f build/rpm/packages/*.src.rpm
        rpm -i build/rpm/packages/*.rpm
    else
        echo "unrecognized pkg source!"
        exit 1
    fi
fi

# verify installed version
INSTALLED=$(/opt/xrpl/bin/xrpld --version | awk '{print $NF}')
if [ "${xrpld_version}" != "${INSTALLED}" ] ; then
    echo "INSTALLED version ${INSTALLED} does not match ${xrpld_version}"
    exit 1
fi
# run unit tests
/opt/xrpl/bin/xrpld --unittest --unittest-jobs $(nproc)
/opt/xrpl/bin/validator-keys --unittest


