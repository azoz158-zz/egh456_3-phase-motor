# invoke SourceDir generated makefile for i2ctmp006.pem4f
i2ctmp006.pem4f: .libraries,i2ctmp006.pem4f
.libraries,i2ctmp006.pem4f: package/cfg/i2ctmp006_pem4f.xdl
	$(MAKE) -f H:\assignment\i2ctmp006_DK_TM4C129X_TImine/src/makefile.libs

clean::
	$(MAKE) -f H:\assignment\i2ctmp006_DK_TM4C129X_TImine/src/makefile.libs clean

