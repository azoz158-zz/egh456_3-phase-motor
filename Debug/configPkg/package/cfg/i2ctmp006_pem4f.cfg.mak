# invoke SourceDir generated makefile for i2ctmp006.pem4f
i2ctmp006.pem4f: .libraries,i2ctmp006.pem4f
.libraries,i2ctmp006.pem4f: package/cfg/i2ctmp006_pem4f.xdl
	$(MAKE) -f H:\WEEK2\TivaWare_C_Series-2.1.3.156\examples\boards\dk-tm4c129x\egh456/src/makefile.libs

clean::
	$(MAKE) -f H:\WEEK2\TivaWare_C_Series-2.1.3.156\examples\boards\dk-tm4c129x\egh456/src/makefile.libs clean

