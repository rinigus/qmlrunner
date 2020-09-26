Name:       qmlrunner

Summary:    Runner for pure QML applications
Version:    1.0.0
Release:    1
Group:      Qt/Qt
License:    GPLv3
URL:        https://github.com/rinigus/qmlrunner
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)

%description
%summary


%prep
%setup -q -n %{name}-%{version}

%build

%qmake_qt5 PREFIX=%{_prefix}
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root,-)
%{_bindir}/qmlrunner
