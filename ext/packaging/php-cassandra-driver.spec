%define distnum %(/usr/lib/rpm/redhat/dist.sh --distnum)
%global ini_name   40-cassandra.ini

Name:    %{package_name}
Epoch:   1
Version: %{driver_version}
Release: 1%{?dist}
Summary: PHP Cassandra Extension

Group: Development/Tools
License: Proprietary
URL: https://github.com/ciaran-moore/php-cassandra-driver
Source0: %{name}-%{version}.tar.gz

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
%if %{distnum} == 5
BuildRequires: buildsys-macros >= 5
%endif
BuildRequires: cassandra-cpp-driver-devel >= 2.17.0
BuildRequires: php-devel >= 8.0
BuildRequires: gmp-devel

Requires: php(zend-abi) = %{php_zend_api}
Requires: php(api) = %{php_core_api}
Requires: cassandra-cpp-driver >= 2.17.0
Requires: gmp

%description
A feature-rich and highly tunable PHP client library for
Apache Cassandra (3+) using Cassandra's binary protocol and Cassandra 
Query Language v3.

%prep
%setup -qn %{name}-%{version}

cat << 'EOF' | tee  %{ini_name}
; Enable %{summary} extension module
extension=cassandra.so
EOF

%build
%{_bindir}/phpize
export CFLAGS=-Wno-extended-offsetof
%configure \
    --with-cassandra \
    --with-libdir=%{_lib} \
    --with-php-config=%{_bindir}/php-config
make %{?_smp_mflags}

%install
make install INSTALL_ROOT=%{buildroot}
install -D -m 644 %{ini_name} %{buildroot}%{php_inidir}/%{ini_name}

%clean
rm -rf %{buildroot}

%check
# make check

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%config(noreplace) %{php_inidir}/%{ini_name}
%{php_extdir}/cassandra.so

%changelog
- release
