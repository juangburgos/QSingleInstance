# Usage

## Add to project

Add ```qsingleinstance.pri``` to your application's project file (Qt's pro file), e.g.

```bash
# other stuff

include(./QSingleInstance.git/src/qsingleinstance.pri)

# other stuff
```

## Add to source

An instance of the ```QSingleInstance``` class must be added to the ```main.cpp``` file of your application. The instance must be added right **after** the ```QApplication``` instantiation, but **before** the instantiation of any other cusotm widget. E.g.

```cplusplus
#include <QApplication>
#include <QMessageBox>
#include "dialog.h"

// Add single instance header
#include <QSingleInstance>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Apply guard after QApplication instantiation
    // but before instantiating any custom widgets
    QSingleInstance guard("MyQDialog");
    if (!guard.tryToRun())
    {
        QMessageBox::critical(nullptr,
            QObject::trUtf8("Single Instance Guard"),
            QObject::trUtf8("An instance of this application is already running.\nClose existng instance before running a new one."),
            QMessageBox::Ok);
        return -1;
    }

    // Custom application dialog
    Dialog w;
    w.show();
    
    return a.exec();
}
```


## Specifics

The ```QSingleInstance``` class creates some temporary files to obtain the single instance locking mechanism. The directory and name of these files are platform dependent. 

The directory in **Windows** is in the system's temporary directory, this is usually the path in the ```TEMP``` or ```TMP``` environment variable.

In **Linux**, this is the path in the ```TMPDIR``` environment variable or ```/tmp``` if ```TMPDIR``` is not defined.

An example of the names of thse files are:

```
qipc_systemsem_beeccbcdeeaefcde296e990973cc975cf92bdb10bdb3bfd583bb2d5d
qipc_systemsem_aefbdbfedfcada7964cb9b08e40a7170e807eac76844af69280cd5
qipc_sharedmemory_aefbdbfedfcada7964cb9b08e40a7170e807eac76844af69280cd5
```

These files are created when the first instance is created. Then remain through the life span of the application. They **must** be automatically deleted when the single instance is closed. Any bugs on the single instance behaviour are notmally related to these files.

# References

* <http://doc.qt.io/qt-5/qdir.html#tempPath>

* <http://habrahabr.ru/post/173281/>

