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
