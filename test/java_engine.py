import jpype 
import jpype.imports
from jpype.types import *


jpype.startJVM(classpath = ['jars/*', 'test/classes'])

#package com.c1games.terminal.game

javaPackage = jpype.JPackage("asdf") 
javaClass = javaPackage.Test 
javaObject = javaClass() 
javaObject.stuff() 
jpype.shutdownJVM()