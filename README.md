## LDAPExplorerTool2

LDAPExplorerTool2 by Laurent Ulrich working with wxWidget 3.0 (Debian 10)

http://ldaptool.sourceforge.net/

LDAPExplorerTool is a multi-platform LDAP browser and editor (GUI). Tested platforms Debian Linux.

I was looking for a lightweight LDAP editor for Linux. In the absence of getting the LDAP Admin \
Linux port working, had an attempt to get this somewhat less featured interface running in a modern \
wxWidget 3.0 environment. 

### Main features listed from original site:

*SSL/TLS support*\
*Full UNICODE support*\
*Create/edit/remove LDAP objects*\
*Multivalue support (including edition)*

### Licence

LDAPExplorerTool was licensed under the BSD license by the original author.

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

