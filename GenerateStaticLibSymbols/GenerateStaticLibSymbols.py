import os
from stat import *
import sys
import time
import string
import time

DEBUG = False

# Global Vars
SourceDirectory = ""
OutputFileH = ""
OutputFileCPP = ""
ValidCall = False
BaseFileName = ""

print "Running Static Lib Generation Script"
#print sys.argv[0]

if DEBUG:
    for arg in sys.argv:
        print arg
    print len(sys.argv)
    SourceDirectory = sys.argv[1]    
    BaseFileName = "StaticLibSymbols"
    OutputFileH = "StaticLibSymbols.h"
    OutputFileCPP = "StaticLibSymbols.cpp"
    ValidCall = True
else:
    if len(sys.argv) == 3:
        if os.path.isdir(sys.argv[1]):
            SourceDirectory = sys.argv[1]
            BaseFileName = sys.argv[2]
            OutputFileH = BaseFileName + ".h"
            OutputFileCPP = BaseFileName + ".cpp"
            ValidCall = True
        else:
            print "ERROR: Bad Argument: Invalid Source Directory"
    else:
        print "ERROR: Invalid Number of Arguments"
        print "USAGE: <SourceDirectory> <BaseOutputFileName>"
        print "EXAMPLE: GenerateStaticLibSymbols.py C:\Development\Plugins2\ElementalEngine\Cal3dModels StaticLibSymbols.cpp"
        print "NOTE: If your directory or file path contains a space, it requres ""."
        print "      If your path ends in an \"\\\" and contains a space you must add"
        print "      an additional \ to correct a Windows Bash bug. This is particularly"
        print "      important in VS2005 as the $(InputDir) contains a trailing \\"
        print "      use \"$(InputDir)\\\" instead of \"$(InputDir)\""
        
def ParseFolder(dirname, filelist):
    if not os.path.isdir(dirname):
        print 'DIR INPUT: ' + dirname + ' IS EPIC FAIL!!!'
        return
    filelist = []
    for fname in os.listdir(dirname):
        if fname.lower().endswith('.cpp'):
            filelist.append(fname)
            #print "parsing: " + fname
    return filelist

def ParseDLLInfoLine(line):
    if "INITDLLINFO" in line:
        partline = line.strip()
        if (partline[0] == '/') and (partline[1] == '/'):
            return False, line
        tokens = line.split(" ")
        if len(tokens) >= 3:
            tokens = tokens[1].split("_")
            if len(tokens) >= 2 and "INITDLLINFO" in tokens[1]:
                return True, tokens[0].strip()
    return False, line
    
def ParseFileForDLLInfo(filepath, parsedmessages):
    if not os.path.isfile(filepath):
        print 'FILE INPUT: ' + filepath + ' IS EPIC FAIL!!!'
        return
    F = open(filepath)
    FLines = F.readlines()
    F.close()
    for line in FLines:
        returnval, returnline = ParseDLLInfoLine(line)
        if returnval:
            parsedmessages.append(returnline)
    return parsedmessages
    
def ParseComponentLine(line):
    if "REGISTER_COMPONENT" in line:
        partline = line.strip()
        if (partline[0] == '/') and (partline[1] == '/'):
            return False, line
        preseperator, seperator, partline = line.partition("(")
        partline = partline.strip()
        partline = partline.strip(");")
        partline = partline.strip()
        if "REGISTER_COMPONENT_AS" in line:
            tokens = partline.split(",")
            partline = tokens[0]
            partline = partline.strip()
        return True, partline
    return False, line

def ParseFileForComponents(filepath, parsedcomponents):
    if not os.path.isfile(filepath):
        print 'FILE INPUT: ' + filepath + ' IS EPIC FAIL!!!'
        return
    F = open(filepath)
    FLines = F.readlines()
    F.close()
    for line in FLines:
        returnval, returnline = ParseComponentLine(line)
        if returnval:
            parsedcomponents.append(returnline)
    return parsedcomponents

def ParseSingletonLine(line):
    if "REGISTER_COMPONENT_SINGLETON" in line:
        partline = line.strip()
        if (partline[0] == '/') and (partline[1] == '/'):
            return False, line
        preseperator, seperator, partline = line.partition("(")
        partline = partline.strip()
        partline = partline.strip(");")
        partline = partline.strip()
        if "REGISTER_COMPONENT_SINGLETON_AS" in line:
            tokens = partline.split(",")
            partline = tokens[0]
            partline = partline.strip()
        return True, partline
    return False, line

def ParseFileForSingletons(filepath, parsedsingletons):
    if not os.path.isfile(filepath):
        print 'FILE INPUT: ' + filepath + ' IS EPIC FAIL!!!'
        return
    F = open(filepath)
    FLines = F.readlines()
    F.close()
    for line in FLines:
        returnval, returnline = ParseSingletonLine(line)
        if returnval:
            parsedsingletons.append(returnline)
    return parsedsingletons

def ParseMessageLine(line):
    if "REGISTER_MESSAGE_HANDLER" in line:
        partline = line.strip()
        if (partline[0] == '/') and (partline[1] == '/'):
            return False, line
        tokens = line.split(")")
        if len(tokens) >= 1:
            tokens = tokens[0].split("(")
            if len(tokens) >= 2:
                tokens = tokens[1].split(",")
                if len(tokens) >= 3:
                    partline = tokens[0].strip() + "_" + tokens[2].strip()
                    #print partline
                    return True, partline
    return False, line

def ParseFileForMessages(filepath, parsedmessages):
    if not os.path.isfile(filepath):
        print 'FILE INPUT: ' + filepath + ' IS EPIC FAIL!!!'
        return
    F = open(filepath)
    FLines = F.readlines()
    F.close()
    for line in FLines:
        returnval, returnline = ParseMessageLine(line)
        if returnval:
            parsedmessages.append(returnline)
    return parsedmessages

def WriteFile(filepath, FLines):
#    if not os.path.isfile(filepath):
#        print 'FILE OUTPUT: ' + filepath + ' IS EPIC FAIL!!!'
#        return
    F = open(filepath, 'w')
    filetime = time.strftime("%x %X", time.localtime() )
    FLines.insert(0, "//Generated on: " + filetime + "\n\n")
    FLines.insert(0, "//Autogenerated Static Lib File:\n")
    F.writelines(FLines)
    F.close()  

if ValidCall == True:
    SourceFiles = []
    SourceFiles = ParseFolder(SourceDirectory, SourceFiles)

    #ParseForComponents
    ParsedComponents = []
    for SourceFile in SourceFiles:
        #Do not Parse Output File.
        if not SourceFile == OutputFileCPP:
            ParseFileForComponents(SourceDirectory + "\\" + SourceFile, ParsedComponents)

    #ParseForMessages
    ParsedMessages = []
    for SourceFile in SourceFiles:
        #Do not Parse Output File.
        if not SourceFile == OutputFileCPP:
            ParseFileForMessages(SourceDirectory + "\\" + SourceFile, ParsedMessages)

    #ParseForSingletons
    ParsedSingletons = []
    for SourceFile in SourceFiles:
        #Do not Parse Output File.
        if not SourceFile == OutputFileCPP:
            ParseFileForSingletons(SourceDirectory + "\\" + SourceFile, ParsedSingletons)
    
    #ParseForDLLInitInfo
    ParsedDLLInitInfo = []
    for SourceFile in SourceFiles:
        #Do not Parse Output File.
        if not SourceFile == OutputFileCPP:
            ParseFileForDLLInfo(SourceDirectory + "\\" + SourceFile, ParsedDLLInitInfo)
            
    #Format Output
    PrintLines = []
    
    PrintLines.append("\n#include \".\\" + OutputFileH + "\"\n")
    
    PrintLines.append("#ifdef _LIB\n\n")
    PrintLines.append("\nvoid *" + ParsedDLLInitInfo[0] + "_LIBEXTERNS[] = {\n")
    
    #Output singleton list
    for parsedline in ParsedSingletons:
        parsedline = "\t(void *)&" + parsedline + "RO,\n"
        PrintLines.append(parsedline)
        
    #(void *)$CLASSNAME$RO
    for parsedline in ParsedComponents:
        parsedline = "\t(void *)&" + parsedline + "RO,\n"
        PrintLines.append(parsedline)
        
    #(void *)$MESSAGENAME$RM
    for parsedline in ParsedMessages:
        parsedline = "\t(void *)&" + parsedline + "RM,\n"
        PrintLines.append(parsedline)

    PrintLines.append("\t(void *)0\n};\n\n")
    	
    PrintLines.append("#endif\t//#ifdef _LIB\n\n")
    WriteFile(SourceDirectory + "\\" + OutputFileCPP, PrintLines)
    
    PrintLines = []
    
    for parsedline in ParsedDLLInitInfo:
        PrintLines.append("#ifndef INCLUDE_SYMBOLS_" + parsedline + "\n")
        PrintLines.append("#define INCLUDE_SYMBOLS_" + parsedline + "\n")
        PrintLines.append("#endif\t//#ifndef INCLUDE_SYMBOLS_" + parsedline + "\n")

    PrintLines.append("#ifdef _LIB\n\n")
    PrintLines.append("namespace ElementalEngine {\nstruct INITDLLINFO;\n")
    PrintLines.append("class CRegisterComponent;\n")
    PrintLines.append("class CRegisterMessage;\n")
    PrintLines.append("}\n\nusing namespace ElementalEngine;\n\n")
    
    # there really should be only one...
    PrintLines.append("\nextern void *" + ParsedDLLInitInfo[0] + "_LIBEXTERNS[];\n")
    PrintLines.append("//Singleton externs\n")
    #Single Component Extern stuff
    for parsedline in ParsedSingletons:
        PrintLines.append("extern \"C\" CRegisterComponent " + parsedline + "RO;\n")
                
    PrintLines.append("//Component externs\n")
    #Component Extern stuff
    for parsedline in ParsedComponents:
        PrintLines.append("extern \"C\" CRegisterComponent " + parsedline + "RO;\n")
        
    PrintLines.append("//Message externs\n")
    #Message Extern stuff
    for parsedline in ParsedMessages:
        PrintLines.append("extern \"C\" CRegisterMessage " + parsedline + "RM;\n")

    PrintLines.append("\n#endif\t//#ifdef _LIB\n")
    
    WriteFile(SourceDirectory + "\\" + OutputFileH, PrintLines)

