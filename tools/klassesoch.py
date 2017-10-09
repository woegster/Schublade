import argparse

def getFunctionInfo(decl):
  startOfParams = decl.index("(")
  endOfParams = decl.rindex(")")
  
  beforeArgs = decl[0:startOfParams]
  afterArgs = decl[endOfParams+1:]
  
  parametersDecl = decl[startOfParams+1:endOfParams]
  
  info = {};
  info["params"] = parametersDecl.split(", ")
  
  prefixComponents = beforeArgs.split() 
  
  info["suffix"] = afterArgs.strip()
  info["name"] = prefixComponents[-1]
  
  info["prefix"] = ""
  info["retType"] = ""
  
  for cmpt in prefixComponents[0:-1]:
    if cmpt == "virtual" or cmpt == "static" or cmpt == "inline":
      info["prefix"] += cmpt + " "
    else:
      info["retType"] += cmpt + " "
  
  info["prefix"] = info["prefix"].strip();
  info["retType"] = info["retType"].strip();
    
  info["isPureVirtual"] = info["suffix"].endswith("= 0")
  if info["prefix"].find("inline") != -1:
    info["isInline"] = True
  else:
    info["isInline"] = False
    
  return info
  
def makeHeaderDeclerationFromFunctionInfo(functionInfo):
  declLine = "  "
  if functionInfo["prefix"] != "":
    declLine += functionInfo["prefix"] + " "
  declLine += " ".join([functionInfo["retType"], functionInfo["name"]])
  declLine += "("
  reqIdentLen = len(declLine)
  if len(functionInfo["params"]) >= 1:
    declLine += functionInfo["params"][0]
    for param in functionInfo["params"][1:]:
      declLine += str.format(",\n{0}{1}", reqIdentLen*" ", param)    
  declLine += ")"
  
  if functionInfo["suffix"] != "":
    declLine += " " + functionInfo["suffix"]
  return declLine
  
def makeSourceDeclerationFromFunctionInfo(functionInfo, classname):
  declLine = str.format("{0} {1}::{2}(", functionInfo["retType"], classname, functionInfo["name"])
  reqIdentLen = len(declLine)
  if len(functionInfo["params"]) >= 1:
    declLine += functionInfo["params"][0]
    for param in functionInfo["params"][1:]:
      declLine += str.format(",\n{0}{1}", reqIdentLen*" ", param)    
  declLine += ")"
  
  if functionInfo["suffix"] != "":
    declLine += " " + functionInfo["suffix"]
  return declLine.replace("override", "")

def generateHeaderForClass(classname, isMoveable, isCopyable, isFinal, baseClass, virtualDtor, pureVirtualDtor, members, valueMembers, functions):
  cppCode = "class " + classname
  if isFinal:
    cppCode += " final"
	
  if baseClass != None:
    cppCode += " : public " + baseClass
  
  cppCode += "\n{\npublic:\n";
  if not pureVirtualDtor:    
    #ctor
    if members == None:
      cppCode += str.format("  {0}();\n", classname);
    else:
      cppCode += str.format("  {0}({1}_", classname, members[0]);
      for member in members[1:]:
        cppCode += str.format(",\n{0}{1}_", (3+len(classname))*" ", member)    
      cppCode += str.format(");\n");
    
    #move ctor
    cppCode += str.format("  {0}({0}&& moveFrom)", classname);  
    if not isMoveable:
      cppCode += " = delete";
    cppCode += ";\n";
    
    #move assign
    cppCode += str.format("  {0}& operator=({0}&& moveFrom)", classname)
    if not isMoveable:
      cppCode += " = delete";
    cppCode += ";\n";
    
    #copy ctor
    cppCode += str.format("  {0}(const {0}& copyFrom)", classname);  
    if not isCopyable:
      cppCode += " = delete";
    cppCode += ";\n";
    
    #copy assign
    cppCode += str.format("  {0}& operator=(const {0}& copyFrom)", classname)
    if not isCopyable:
      cppCode += " = delete";
    cppCode += ";\n";
    
    if virtualDtor:
      cppCode += str.format("  virtual ~{0}() = default;\n", classname)
  else:
    cppCode += str.format("  virtual ~{0}() = 0;\n", classname)
      
  if functions != None:
    cppCode += "public:\n"
    for function in functions:
      info = getFunctionInfo(function)
      declLine = makeHeaderDeclerationFromFunctionInfo(info)
      if info["isInline"] == True:
        cppCode += str.format("{0}\n  {{\n  }}\n", declLine)
      else:
        cppCode += str.format("{0};\n", declLine)
      

  if not pureVirtualDtor:
    if members != None or valueMembers != None:
      cppCode += "private:\n"
      if members != None:
        for member in members:
          cppCode += str.format("  {0};\n", member);
      if valueMembers != None:
        for member in valueMembers:
          cppCode += str.format("  {0};\n", member);
  
  cppCode += "};\n";
  return cppCode;
  
def getNameOfMember(member):
  return member.split()[-1]



  
def generateSourceForClass(classname, isMoveable, isCopyable, pureVirtualDtor, members, functions, memberyByValue):
  cppCode = ""
  allMembers = []
  if members != None:
    allMembers += members
  if memberyByValue != None:
    allMembers += memberyByValue
    
  if not pureVirtualDtor:
    #ctor
    if members == None:
      cppCode = str.format("{0}::{0}()\n{{\n}}\n\n", classname)
    else:
      cppCode = str.format("{0}::{0}({1}_", classname, members[0])
      for member in members[1:]:
        cppCode += str.format(",\n{0}{1}_", (3+(len(classname)*2))*" ", member)    
      cppCode += str.format(")\n");
      cppCode += str.format("  : {0}({1})\n", getNameOfMember(members[0]), getNameOfMember(members[0]) + "_");
      for member in members[1:]:
        cppCode += str.format("  , {0}({1})\n", getNameOfMember(member), getNameOfMember(member) + "_");
      cppCode += "{\n}\n\n";
    
    if isMoveable:
      #move ctor    
      cppCode += str.format("{0}::{0}({0}&& moveFrom)\n", classname)
      if allMembers != None:
        cppCode += str.format("  : {0}(std::move(moveFrom.{0}))\n", getNameOfMember(allMembers[0]));
        for member in allMembers[1:]:
          cppCode += str.format("  , {0}(std::move(moveFrom.{0}))\n", getNameOfMember(member));
      cppCode += "{\n}\n\n"
      #move assign
      cppCode += str.format("{0}& {0}::operator=({0}&& moveFrom)\n{{\n", classname)
      if allMembers != None:
        for member in allMembers:
          cppCode += str.format("  {0} = std::move(moveFrom.{0});\n", getNameOfMember(member))
      cppCode += str.format("  return *this;\n}}\n\n")
        
    if isCopyable:
      #copy ctor
      cppCode += str.format("{0}::{0}(const {0}& copyFrom)\n", classname)
      if allMembers != None:
        cppCode += str.format("  : {0}(copyFrom.{0})\n", getNameOfMember(allMembers[0]));
        for member in allMembers[1:]:
          cppCode += str.format("  , {0}(copyFrom.{0})\n", getNameOfMember(member));
      cppCode += "{\n}\n\n"      
      #copy assign
      cppCode += str.format("{0}& {0}::operator=(const {0}& copyFrom)\n{{\n", classname)
      if allMembers != None:
        for member in allMembers:
          cppCode += str.format("  {0} = copyFrom.{0};\n", getNameOfMember(member))
      cppCode += str.format("  return *this;\n}}\n\n")
  
  if functions != None:
    for function in functions:
      info = getFunctionInfo(function)
      if info["isInline"] == False and info["isPureVirtual"] == False:
        cppCode += makeSourceDeclerationFromFunctionInfo(info, classname) + "\n{\n}\n\n"
     
  return cppCode
  


parser = argparse.ArgumentParser()
parser.add_argument("classname", help="name of the class")
parser.add_argument("-m", help="moveable", action='store_true', dest="moveable")
parser.add_argument("-c", help="copyable", action='store_true', dest="copyable")
parser.add_argument("-vd", help="virtual destructor", action='store_true', dest="virtualDtor")
parser.add_argument("-pvd", help="pure virtual destructor", action='store_true', dest="pureVirtualDtor")
parser.add_argument("-f", help="final", action='store_true', dest="final")
parser.add_argument("-b", help="inherit form baseclass", dest="baseclass")
parser.add_argument("-hxx", help="fileextension for header", dest="headerExtension", default="hxx")
parser.add_argument("-cxx", help="fileextension for source", dest="sourceExtension", default="cxx")
parser.add_argument("-Mref", help="members of the class given by construction", dest="membersByRef", nargs='*')
parser.add_argument("-Mval", help="members of the class", dest="membersByValue", nargs='*')
parser.add_argument("-F", help="functions fo the class", dest="functions", nargs='*')
parser.add_argument("-ns", help="enclosing namespace for class", dest="namespace")

options = parser.parse_args()

if options.final and options.virtualDtor:
  print("class is final (-f) and has a virtual destructor (-vd) - this makes no sense")
  raise RuntimeError("invalid configuration")
  
if options.pureVirtualDtor and options.virtualDtor:
  print("class has pure virtual destructor (-f) and has a virtual destructor (-vd) - this makes no sense")
  raise RuntimeError("invalid configuration")

headerFileName = options.classname + "." + options.headerExtension
sourceFileName = options.classname + "." + options.sourceExtension

print("Writing to header file " + headerFileName)
headerCode = "#pragma once\n\n"
if options.namespace != None:
  headerCode += "namespace " + options.namespace + "\n"
  headerCode += "{\n\n"  
headerCode += generateHeaderForClass(options.classname, options.moveable, options.copyable, options.final, options.baseclass, options.virtualDtor, options.pureVirtualDtor, options.membersByRef, options.membersByValue, options.functions)
if options.namespace != None:
  headerCode += "\n}\n"
print (headerCode)
with open(headerFileName, "w") as headerFile:
  headerFile.write(headerCode)

print ("Writing to source file " + sourceFileName)
sourceCode = str.format("#include \"{0}\"\n\n", headerFileName)
if options.namespace != None:
  sourceCode += "namespace " + options.namespace + "\n"
  sourceCode += "{\n\n"  
sourceCode += generateSourceForClass(options.classname, options.moveable, options.copyable, options.pureVirtualDtor, options.membersByRef, options.functions, options.membersByValue)
if options.namespace != None:
  sourceCode += "}\n"
print (sourceCode)
with open(sourceFileName, "w") as sourceFile:
  sourceFile.write(sourceCode)

