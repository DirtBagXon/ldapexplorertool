
include ../GNUmakefile.config

CPPFLAGS=$(WX_INC) $(LDAP_INC) -DNDEBUG -DLDAP_DEPRECATED
LDLIBS=$(WX_LIB) $(LDAP_LIB) -lstdc++

OBJS = resource.o application.o mainframe.o configurationdlg.o config_fct.o newentrydlg.o attribute_add_dlg.o schemadlg.o\
	config_manage_dlg.o searchdlg.o ldapclass.o ldapentry.o schema.o

$(TOOLNAME): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LDLIBS)


clean:
	-rm *.o
	-rm $(TOOLNAME)
