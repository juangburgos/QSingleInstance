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

# References

http://habrahabr.ru/post/173281/

