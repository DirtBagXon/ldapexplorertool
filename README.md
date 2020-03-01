## LDAPExplorerTool

LDAPExplorerTool2 by Laurent Ulrich working with wxWidget 3.0 including several fixes and enhancements.

http://ldaptool.sourceforge.net/

LDAPExplorerTool is an LDAP browser and editor (GUI). Tested platforms Linux (Debian 9/10).

I was searching for a lightweight LDAP editor for Linux. Refusing to install a large IDE for the LDAP Admin \
Linux port, I attempted to get this somewhat abandoned, lesser featured, application running in a modern \
environment with wxWidget 3.0. It is now operating in a manner that is usable and remains lightweight.

This editor/browser offers a fairly raw interface into the world of LDAP, objectClasses and schemas. \
Some understanding of these topics is beneficial, or this utility can be a great tool for learning them.

### Main features listed from original site:

*SSL/TLS support*\
*Full UNICODE support*\
*Create/edit/remove LDAP objects*\
*Multivalue support (including edition)*

### Licencing

Original LDAPExplorerTool code was licensed under the BSD license by the author. \
Modifications within this repository are released under GPL-2.

### Required libraries

    Binary: libwxgtk3.0-0v5 wx-common libldap-common

    Build:  g++ make libwxbase3.0-dev libwxgtk3.0-dev wx-common wx3.0-headers libldap2-dev

### Build

    make
    sudo make install

### Usage

The binary expects the `.xrc` file in `/usr/share/ldapexplorertool2/ldapexplorertool2.xrc` by default, \
or it will error out. The `make install` will remedy this. However you can execute a local copy by giving \
a `directory` name containing the `ldapexplorertool2.xrc` file via the following argument:

    ldapexplorertool2 -xrc <dir>

Other available command line options:

    Usage: ldapexplorertool2 [-c <str>] [-xrc <str>] [-h]
     -c, --config-file=<str>       Configuration file to load
     -xrc, --xrc-dir=<str>         Directory with interface definition file to load
     -h, --help                    Usage of LDAPExplorerTool


### Screenshots

![ldapexplorer](https://raw.githubusercontent.com/DirtBagXon/ldapexplorer/master/images/ldapexplorer1.png)
![ldapexplorer](https://raw.githubusercontent.com/DirtBagXon/ldapexplorer/master/images/ldapexplorer2.png)
![ldapexplorer](https://raw.githubusercontent.com/DirtBagXon/ldapexplorer/master/images/ldapexplorer3.png)

