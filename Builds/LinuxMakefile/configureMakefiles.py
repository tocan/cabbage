import subprocess
import sys

print "================================================="
print "======== Generate Makefiles for Cabbage ========="
print "================================================="
print ""
print "This script will generate makefiles for Cabbage by"
print "running the ProJUCEr application. Please make sure"
print "you have already built the JUCE Projucer."
print ""
print "Usage:"
print "python ./projucerConfig [../path_to/projucer]"
print ""
print "python configureMakefiles.py ../../../JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer"
print ""
print "More info..."
print "The standalone and plugin version of Cabbage use slightly different"
print "JUCE and makefile configurations. This script creates makefiles "
print "using the Projucer, and then modifies them. The result is a single"
print "code-base across both projects, and project makefiles that can be "
print "called without needing to constantly use the Projucer"


if len(sys.argv)==1:
	pathToJucer = "../../../JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer"
else:
	pathToJucer = sys.argv[1];

createPluginCommand = str(pathToJucer)+ " --resave ../../CabbagePlugin.jucer"
process = subprocess.Popen(createPluginCommand, shell=True, stdout=subprocess.PIPE)
process.wait()

filedata = None
with open("Makefile", "rt") as file:
	filedata = file.read()

	filedata = filedata.replace('CabbagePlugin.so', 'CabbagePluginSynth.so')
	filedata = filedata.replace('@echo Cleaning CabbagePlugin', '@echo Cleaning CabbagePluginSynth')
	filedata = filedata.replace('@echo Linking CabbagePlugin', '@echo Linking CabbagePluginSynth')

	# Write the file out again
	with open('MakePluginSynth', 'w') as file:
	  file.write(filedata)

with open("Makefile", "rt") as file:
	filedata = file.read()

	filedata = filedata.replace('-DCabbage_Plugin_Synth=1', '')
	filedata = filedata.replace('CabbagePlugin.so', 'CabbagePluginEffect.so')
	filedata = filedata.replace('@echo Cleaning CabbagePlugin', '@echo Cleaning CabbagePluginEffect')
	filedata = filedata.replace('@echo Linking CabbagePlugin', '@echo Linking CabbagePluginEffect')

	# Write the file out again
	with open('MakePluginEffect', 'w') as file:
	  file.write(filedata)

createIDECommand = str(pathToJucer)+ " --resave ../../CabbageIDE.jucer"
process = subprocess.Popen(createIDECommand, shell=True, stdout=subprocess.PIPE)
process.wait()

with open("Makefile", "rt") as file:
	lines = file.read().splitlines()
	lines.pop(426)
	lines.pop(425)
	lines.pop(424)
	lines.pop(423)
	lines.pop(114)
	# Write the file out again
	with open('MakeCabbageIDE', 'w') as file:
		file.write('\n'.join(lines))