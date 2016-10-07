/* 
 * File:   IOError.cpp
 * Author: User
 * 
 * Created on 15 Январь 2012 г., 2:37
 */

#include "IOError.h"

IOError::IOError() {
    this->error = 0;
    this->message.assign("no message");
}

IOError::IOError(int errCode){
    this->error = errCode;
    this->message.assign("no message");
}

IOError::IOError(int errCode,const String& message){
    this->error = errCode;
    this->message.assign(message);
}

IOError::IOError(const IOError& orig) {
    message = orig.message;
}

IOError::~IOError() {
}

String IOError::getMessage(){
    return message;
}

int IOError::getError(){
    return error;
}
