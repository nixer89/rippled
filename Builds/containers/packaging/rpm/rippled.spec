%define rippled_version %(echo $RIPPLED_RPM_VERSION)
%define rpm_release %(echo $RPM_RELEASE)
%define rpm_patch %(echo $RPM_PATCH)
%define _prefix /opt/xrpl
Name:           xrpld
# Dashes in Version extensions must be converted to underscores
Version:        %{rippled_version}
Release:        %{rpm_release}%{?dist}%{rpm_patch}
Summary:        xrpld daemon

License:        MIT
URL:            http://ripple.com/
Source0:        xrpld.tar.gz

BuildRequires:  cmake zlib-static ninja-build

%description
xrpld

%package devel
Summary: Files for development of applications using xrpl core library
Group: Development/Libraries
Requires: openssl-static, zlib-static

%description devel
core library for development of standalone applications that sign transactions.

%prep
%setup -c -n xrpld

%build
cd xrpld
mkdir -p bld.release
cd bld.release
cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_BUILD_TYPE=Release -DCMAKE_UNITY_BUILD_BATCH_SIZE=10 -Dstatic=true -DCMAKE_VERBOSE_MAKEFILE=ON -Dvalidator_keys=ON
cmake --build . --parallel --target xrpld --target validator-keys -- -v

%pre
test -e /etc/pki/tls || { mkdir -p /etc/pki; ln -s /usr/lib/ssl /etc/pki/tls; }

%install
rm -rf $RPM_BUILD_ROOT
DESTDIR=$RPM_BUILD_ROOT cmake --build xrpld/bld.release --target install -- -v
rm -rf ${RPM_BUILD_ROOT}/%{_prefix}/lib64/cmake/date
install -d ${RPM_BUILD_ROOT}/etc/opt/xrpl
install -d ${RPM_BUILD_ROOT}/usr/local/bin
ln -s %{_prefix}/etc/xrpld.cfg ${RPM_BUILD_ROOT}/etc/opt/xrpl/xrpld.cfg
ln -s %{_prefix}/etc/validators.txt ${RPM_BUILD_ROOT}/etc/opt/xrpl/validators.txt
ln -s %{_prefix}/bin/xrpld ${RPM_BUILD_ROOT}/usr/local/bin/xrpld
install -D xrpld/bld.release/validator-keys/validator-keys ${RPM_BUILD_ROOT}%{_bindir}/validator-keys
install -D ./xrpld/Builds/containers/shared/xrpld.service ${RPM_BUILD_ROOT}/usr/lib/systemd/system/xrpld.service
install -D ./xrpld/Builds/containers/packaging/rpm/50-xrpld.preset ${RPM_BUILD_ROOT}/usr/lib/systemd/system-preset/50-xrpld.preset
install -D ./xrpld/Builds/containers/shared/update-xrpld.sh ${RPM_BUILD_ROOT}%{_bindir}/update-xrpld.sh
install -D ./xrpld/bin/getRippledInfo ${RPM_BUILD_ROOT}%{_bindir}/getRippledInfo
install -D ./xrpld/Builds/containers/shared/update-xrpld-cron ${RPM_BUILD_ROOT}%{_prefix}/etc/update-xrpld-cron
install -D ./xrpld/Builds/containers/shared/xrpld-logrotate ${RPM_BUILD_ROOT}/etc/logrotate.d/xrpld
install -d $RPM_BUILD_ROOT/var/log/xrpld
install -d $RPM_BUILD_ROOT/var/lib/xrpld

%post
USER_NAME=xrpld
GROUP_NAME=xrpld

getent passwd $USER_NAME &>/dev/null || useradd $USER_NAME
getent group $GROUP_NAME &>/dev/null || groupadd $GROUP_NAME

chown -R $USER_NAME:$GROUP_NAME /var/log/xrpld/
chown -R $USER_NAME:$GROUP_NAME /var/lib/xrpld/
chown -R $USER_NAME:$GROUP_NAME %{_prefix}/

chmod 755 /var/log/xrpld/
chmod 755 /var/lib/xrpld/

chmod 644 %{_prefix}/etc/update-xrpld-cron
chmod 644 /etc/logrotate.d/xrpld
chown -R root:$GROUP_NAME %{_prefix}/etc/update-xrpld-cron

%files
%doc xrpld/README.md xrpld/LICENSE.md
%{_bindir}/xrpld
/usr/local/bin/xrpld
%{_bindir}/update-xrpld.sh
%{_bindir}/getRippledInfo
%{_prefix}/etc/update-xrpld-cron
%{_bindir}/validator-keys
%config(noreplace) %{_prefix}/etc/xrpld.cfg
%config(noreplace) /etc/opt/xrpl/xrpld.cfg
%config(noreplace) %{_prefix}/etc/validators.txt
%config(noreplace) /etc/opt/xrpl/validators.txt
%config(noreplace) /etc/logrotate.d/xrpld
%config(noreplace) /usr/lib/systemd/system/xrpld.service
%config(noreplace) /usr/lib/systemd/system-preset/50-xrpld.preset
%dir /var/log/xrpld/
%dir /var/lib/xrpld/

%files devel
%{_prefix}/include
%{_prefix}/lib/*.a
%{_prefix}/lib/cmake/xrpl

%changelog
* Wed Aug 28 2019 Mike Ellery <mellery451@gmail.com>
- Switch to subproject build for validator-keys

* Wed May 15 2019 Mike Ellery <mellery451@gmail.com>
- Make validator-keys use local xrpld build for core lib

* Wed Aug 01 2018 Mike Ellery <mellery451@gmail.com>
- add devel package for signing library

* Thu Jun 02 2016 Brandon Wilson <bwilson@ripple.com>
- Install validators.txt

