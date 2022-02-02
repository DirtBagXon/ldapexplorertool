## LDAPExplorerTool

LDAPExplorerTool2 by Laurent Ulrich working with wxWidget 3.0/3.1, includes several fixes and enhancements.

http://ldaptool.sourceforge.net/

LDAPExplorerTool is an LDAP browser and editor (GUI).

Tested linux platforms:

    Debian 9  - wxWidgets 3.0.2 / g++ 6.3.0 / gtk2
    Debian 10 - wxWidgets 3.0.4 / g++ 8.3.0 / gtk2
    Debian 10 - wxWidgets 3.1.3 / g++ 8.3.0 / gtk3

I was searching for a lightweight LDAP editor for Linux. Refused to install the large IDE used for the Linux \
port of LDAP Admin. So sat down to adapt this abandoned, lesser featured, LDAP application to run on a modern \
Linux environment. It is now a usable and lightweight application on modern Linux platforms.

I do not have access to Windows build environments, so currently this is not ported, or supported, but I am \
happy to receive pull requests for either platform.

This editor/browser offers a fairly raw interface into the world of LDAP objectClasses and schemas. \
Some understanding of these topics is beneficial, but this utility can be a great tool for learning LDAP.

### Main features listed from original site:

*SSL/TLS support*\
*Full UNICODE support*\
*Create/edit/remove LDAP objects*\
*Multivalue support (including edition)*

### Licensing

Original LDAPExplorerTool code was licensed under the BSD license by the author. \
Modifications within this repository are released under GPL-2.

### Required libraries

    Binary: libwxgtk3.0-0v5 wx-common libldap-common

    Build:  g++ make libwxbase3.0-dev libwxgtk3.0-dev wx-common wx3.0-headers libldap2-dev

- *wxWidgets-3.1.3.tar.bz2* was used for 3.1 testing.


### Build

    wxrc xrc/ldapexplorertool.xrc -c -o src/resource.cc

    make
    sudo make install

#### Ubuntu 20.04 build instructions

1. `git clone git@github.com:DirtBagXon/ldapexplorertool.git`
1. `cd ldapexplorertool/`
1. `sudo apt install wx-common libldap-common libldap2-dev libwxgtk3.0-gtk3-dev`
1. `wxrc xrc/ldapexplorertool.xrc -c -o src/resource.cc`
1. `make`
1. `sudo make install`
1. `./ldapexplorertool`

### Usage

Available command line options:

    Usage: ldapexplorertool [-c <str>] [-h]
     -c, --config-file=<str>       Configuration file to load
     -h, --help                    Usage of LDAPExplorerTool


### Screenshots

![ldapexplorer](https://raw.githubusercontent.com/DirtBagXon/ldapexplorer/master/images/ldapexplorer1.png)
![ldapexplorer](https://raw.githubusercontent.com/DirtBagXon/ldapexplorer/master/images/ldapexplorer2.png)
![ldapexplorer](https://raw.githubusercontent.com/DirtBagXon/ldapexplorer/master/images/ldapexplorer3.png)

