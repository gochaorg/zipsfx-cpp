/* 
 * File:   IOError.h
 * Author: User
 *
 * Created on 15 Январь 2012 г., 2:37
 */

#ifndef _IOERROR_H
#define	_IOERROR_H

#include "String.h"

class IOError {
protected:
    String message;
    int error;
public:
    IOError();
    IOError(int errCode);
    IOError(int errCode,const String& message);
    IOError(const IOError& orig);
    virtual ~IOError();

    String getMessage();
    int getError();
};

#endif	/* _IOERROR_H */

