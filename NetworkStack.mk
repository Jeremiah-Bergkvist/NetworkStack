##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=NetworkStack
ConfigurationName      :=Debug
WorkspacePath          := "/home/jbergkvist/Workspace/c++"
ProjectPath            := "/home/jbergkvist/Workspace/c++/NetworkStack"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Jeremiah Bergkvist
Date                   :=03/20/15
CodeLitePath           :="/home/jbergkvist/.codelite"
LinkerName             :=/usr/bin/g++ 
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="NetworkStack.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -lpthread 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++ 
CC       := /usr/bin/gcc 
CXXFLAGS := -std=c++11 -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/Interface.cpp$(ObjectSuffix) $(IntermediateDirectory)/Machine.cpp$(ObjectSuffix) $(IntermediateDirectory)/Packet.cpp$(ObjectSuffix) $(IntermediateDirectory)/Utilities.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jbergkvist/Workspace/c++/NetworkStack/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/Interface.cpp$(ObjectSuffix): Interface.cpp $(IntermediateDirectory)/Interface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jbergkvist/Workspace/c++/NetworkStack/Interface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Interface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Interface.cpp$(DependSuffix): Interface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Interface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Interface.cpp$(DependSuffix) -MM "Interface.cpp"

$(IntermediateDirectory)/Interface.cpp$(PreprocessSuffix): Interface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Interface.cpp$(PreprocessSuffix) "Interface.cpp"

$(IntermediateDirectory)/Machine.cpp$(ObjectSuffix): Machine.cpp $(IntermediateDirectory)/Machine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jbergkvist/Workspace/c++/NetworkStack/Machine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Machine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Machine.cpp$(DependSuffix): Machine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Machine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Machine.cpp$(DependSuffix) -MM "Machine.cpp"

$(IntermediateDirectory)/Machine.cpp$(PreprocessSuffix): Machine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Machine.cpp$(PreprocessSuffix) "Machine.cpp"

$(IntermediateDirectory)/Packet.cpp$(ObjectSuffix): Packet.cpp $(IntermediateDirectory)/Packet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jbergkvist/Workspace/c++/NetworkStack/Packet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Packet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Packet.cpp$(DependSuffix): Packet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Packet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Packet.cpp$(DependSuffix) -MM "Packet.cpp"

$(IntermediateDirectory)/Packet.cpp$(PreprocessSuffix): Packet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Packet.cpp$(PreprocessSuffix) "Packet.cpp"

$(IntermediateDirectory)/Utilities.cpp$(ObjectSuffix): Utilities.cpp $(IntermediateDirectory)/Utilities.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jbergkvist/Workspace/c++/NetworkStack/Utilities.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Utilities.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Utilities.cpp$(DependSuffix): Utilities.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Utilities.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Utilities.cpp$(DependSuffix) -MM "Utilities.cpp"

$(IntermediateDirectory)/Utilities.cpp$(PreprocessSuffix): Utilities.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Utilities.cpp$(PreprocessSuffix) "Utilities.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


