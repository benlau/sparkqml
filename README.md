# SparkQML

SparkQML is a utility that loads and displays QML document like the qmlscene from Qt SDK. 
However, it would also show the available states of the component and allow to change it on the fly. 
It is useful to have a preview of transition effect between states.

Remarks:  The project is still in an early stage. 
The current version is proof-of-concept prototype. 
If you find any problem, please free free to submit an issue.

![Screenshot](https://raw.githubusercontent.com/benlau/junkcode/master/docs/SparkQML.gif)

Features
========

 1. Load and display QML document
 2. Show available states of the QML document. Allow to change it for present transition effect
 3. Capture screenshot of component to file / clipboard

Command Line Arguments
======================

    sparkqml <filename> [qmlimport.path]

filename - The QML file name

qmlimport.path  - A file that provides the value of QML_IMPORT_PATH. If this parameter is missing, sparkqml would try to search the file started from the document path to root folder.

Integration with Qt Creator
============================

1) Build this project by open app/sparkqml/sparkqml.pro . Copy the executable to somewhere as you like

2) In menu, press "Tools" -> "External" -> "Configure"

3) Then press "Add" -> "Add Tool". Drag the newly created tool under "Qt Quick" category

You may rename it to “SparkQML Viewer"

4) Fill in the following values

```
Executable : The path to the executable

Arguments: %{CurrentDocument:FilePath} %{CurrentProject:Path}/qmlimport.path
```

It is done. Now you could trigger SparkQML while editing a QML file by “Tools” -> "External" -> "Qt Quick" -> "SparkQML Viewer"

QML_IMPORT_PATH
==================

In case SparkQML can’t display your QML file due to “xxx is not installed” error. 
Probably is because it can’t locate your package. 
To fix this problem, you may add the following lines at the end of your pro file:

     write_file(qmlimport.path, QML_IMPORT_PATH)

Then run qmake.
It will create a file called qmlimport.path with the value of QML_IMPORT_PATH.
You may consider to add this file to your .gitignore.

Restart SparkQML. Now it would know where to find packages.

C++ Module
==========

SparkQML do not understand C++ code. 
In case you have referred a component from C++. 
You need to use a mockup project. 
It is a QML project with package and component same as the C++ plugin but it doesn’t need to implement any logic.

<TODO: Explain>
