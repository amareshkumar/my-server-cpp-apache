LDAP_PATH=/cygdrive/d/project/amaresh/srclib/openldap-2.4.40/include
SHA_PATH=/cygdrive/d/project/amaresh/srclib/openssl-0.9.8zc/include/openssl
BERKELEY_PATH=/cygdrive/d/project/amaresh/srclib/db-6.1.19/db-6.1.19.NC/build_windows
NSAPIPATH=/cygdrive/d/project/amaresh/srclib/httpd-2.4.10/include

RSA_PATH=/cygdrive/d/project/amaresh/srclib/openssl-0.9.8zc/include/openssl
#BSAFE_PATH=/usr/cryptoc51
STL_PATH=/cygdrive/d/project/amaresh/srclib/STLport-5.2.1/src

LD_CMD=gcc -g -Wall -G -R$(BERKELEY_PATH)/lib -R$(SHA_PATH)/lib -R$(RSA_PATH) -L$(BERKELEY_PATH)/lib -L$(LDAP_PATH)/lib -L$(SHA_PATH)/lib -L$(BSAFE_PATH)/lib/solaris26

CC_CMD=gcc -finline-functions -mcpu=ultrasparc -Wall -D_TRACE_ENABLED_ -DXP_UNIX -D_STLP_USE_MALLOC -D_STLP_USE_NEWALLOC -D_PTHREADS -D_SOLTHREADS -DREENTRANT -D__USE_MALLOC -D_REENTRANT -I$(STL_PATH) -I$(BERKELEY_PATH)/include -I$(NSAPIPATH) -I$(BSAFE_PATH)/include -I$(LDAP_PATH)/include -I$(SHA_PATH)/include

FLEX_CMD=flex

OBJS=verify_user.o issue.o login_page.o get_back.o ldap_check.o dysplay_page.o ./parser/ParserStructs.o exceptions.o util.o cds_data.o log.o check_vm.o send_mail.o test.o parse_query.o trace.o tstdlib.o manage_func.o logout_page.o xml_response.o remove_usr.o remove_exc.o

all: test.so

test.so: CDS_DATA PROXY_LDAP $(OBJS) 
	$(LD_CMD) $(OBJS) ./cds_data/*.o ./cds_data/cds2/*.o ./cds_data/proxy-ldap/*.o -lthread -lldapssl41 -ldb_cxx -lbsafe -lstdc++ -o test.so

CDS_DATA:
	cd ./cds_data/ && $(MAKE)  && cd ../

PROXY_LDAP:
	cd ./cds_data/proxy-ldap && $(MAKE)  && cd ../../


.SUFFIXES: .cpp 
.cpp.o:
	$(CC_CMD) -c $<

./parser/ParserStructs.o : ./parser/ParserStructs.cpp ./parser/parser.h
	$(CC_CMD) -c -o ./parser/ParserStructs.o ./parser/ParserStructs.cpp

./parser/parser.h : ./parser/Parser.l
	$(FLEX_CMD) -o./parser/parser.h ./parser/Parser.l

clean:
	rm -f *.o all
	rm -f ./parser/*.o
	rm -f test.so
	cd cds_data && $(MAKE) clean
	cd cds_data/proxy-ldap && $(MAKE) clean
	rm -f ./parser/parser.h

