/**

\file
\version 1.1
\date 21/03/2016
\author JGB
\brief Class implementing test to check if there is another application instance running.

*/

#ifndef QSINGLEINSTANCE_H
#define QSINGLEINSTANCE_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

/**

\brief   Class implementing test to check if there is another application instance running.

An instance of this class can be used to ensure that one and only one instance of the application is running.
If another instance is running, then we can decide to close and exit the second instance.
For example in **main.cpp** one can put the code:

```cpp
QSingleInstance guard( "MyId" );
if ( !guard.tryToRun() )
{
	QMessageBox::critical(&w,QObject::trUtf8("Automatic Generation Control"),
	QObject::trUtf8("Ya existe una instancia activa de la aplicación.\nCiérrela antes de arrancar una nueva."),
	QMessageBox::Ok);
	return -1;
}
```

Where "MyId" is a name or identifier of the application.

The mechanism works by creating a shared memory block for all instances if the application.
This is achieved by creating a QSharedMemory on each instance with the same key.
Each time an instance of the application is created we check if the shared memory block
already exists. If it exists it means there is another instance of the application already running.

*/
class QSingleInstance
{
public:
	/**

	Constructor
	\param key Name or identifier of the application.

	*/
    QSingleInstance( const QString& key );

	/**

	Destructor
	Releases the shared memory block.

	*/
    ~QSingleInstance();

	/**

	Check if this is the first and only instance of the application
	\return True if this is the only instance. False if there is another previous instance running.

	*/
    bool tryToRun();


private:
    bool isAnotherRunning();
    void release();

    const QString m_strKey         ;
    const QString m_strMemLockKey  ;
    const QString m_strSharedMemKey;

    QSharedMemory    m_SharedMem  ;
    QSystemSemaphore m_lockSysSemph ;

    Q_DISABLE_COPY( QSingleInstance )
};

#endif // QSINGLEINSTANCE_H
