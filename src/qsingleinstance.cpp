#include "qsingleinstance.h"

#include <QCryptographicHash>


namespace
{

QString generateKeyHash( const QString& key, const QString& salt )
{
    QByteArray data;

    data.append( key.toUtf8() );
    data.append( salt.toUtf8() );
    data = QCryptographicHash::hash( data, QCryptographicHash::Sha1 ).toHex();

    return data;
}

}

QSingleInstance::QSingleInstance(const QString &key)
    : m_strKey          ( key )                                     // get local copy of "key" (i.e. application name)
    , m_strMemLockKey   ( generateKeyHash( key, "_memLockKey" ) )   // generate unique hash based on key
    , m_strSharedMemKey ( generateKeyHash( key, "_sharedmemKey" ) ) // generate another unique hash based on key
    , m_SharedMem       ( m_strSharedMemKey )                       // create a shared piece of memory based on the second hash
    , m_lockSysSemph    ( m_strMemLockKey, 1 )                      // create system-wide semaphore, with a count of "1", based on the first hash
{
	// Attach and detach from shared memory block (on destructor) to avoid linux bug.
	// What this does is create and destroy the memory block, some kind of "reset" mechanism.
	// Reference fix for linux: http://habrahabr.ru/post/173281/
    QSharedMemory fix( m_strSharedMemKey );    
	fix.attach();
}

QSingleInstance::~QSingleInstance()
{
	// Delete memory block if exists.
    release();
}

bool QSingleInstance::tryToRun()
{
	// Check if another is running by checking if the shared memory block already exists
	if (isAnotherRunning())  
	{
        return false;
	}

	// Lock system wide
    m_lockSysSemph.acquire();
	// Create memory block, given that it did not exist before
    const bool result = m_SharedMem.create( sizeof( quint64 ) );
	// Unlock system wide
    m_lockSysSemph.release();

	// If could not create return error, another instance might be running
    if ( !result )
    {
        release();
        return false;
    }
	// If success in creating block, return true
    return true;
}

bool QSingleInstance::isAnotherRunning()
{
	// QSharedMemory::isAttached returns true if this process is attached to the shared memory segment.
	// So, if THIS PROCESS IS ATTACHED, then THERE IS NO OTHER PROCESS RUNNING
    if ( m_SharedMem.isAttached() )
	{
        return false;
	}

	// Lock system wide
	m_lockSysSemph.acquire();

	// Try to attach, if successful it means block already exists, so another is running
	// If unsuccessful it means the memory block has not yet been created
	const bool isRunning = m_SharedMem.attach();

	if (isRunning)
	{
		// Detach if attach was successful, we have no use for the memory block if there is another running
        m_SharedMem.detach();
	}

	// Unlock system wide
    m_lockSysSemph.release();

    return isRunning;
}

void QSingleInstance::release()
{
	// Lock system wide
    m_lockSysSemph.acquire();
	// Detach if attached (delete shared memory block if exists)
	if (m_SharedMem.isAttached())
	{
        m_SharedMem.detach();
	}
	// Unlock system wide
    m_lockSysSemph.release();
}

