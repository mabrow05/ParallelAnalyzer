# Makefile from Jianglai

ObjSuf        = o
SrcSuf        = C
DllSuf        = so
OutPutOpt     = -o  
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)
ROOTCFLAGS    = $(shell root-config --cflags)


# Work with Linux with egcs	
CXX           = g++ 
CXXFLAGS      = -O2 -Wall -fPIC
LD            = g++
SOFLAGS       = -shared
LIBS          = $(ROOTLIBS) $(ROOTGLIBS)
CXXFLAGS     += $(ROOTCFLAGS) -I../include
LIBS         += -lSpectrum -lMinuit

rootfile_translator: rootfile_translator.o
	$(CXX) $(CXXFLAGS) -o rootfile_translator rootfile_translator.o $(LIBS)
	@echo "Done"

# -------------------------------------------------------------------------------
#  Generic compilation and linking step to make an executable from
#  a single *.cc file
#
#%: %.$(SrcSuf)
#	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)
#	@echo "$@ done"

clean:
		@rm -f *.o *~  core
