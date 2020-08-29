#ifndef USERREQUESTSREPOSITORY_H
#define USERREQUESTSREPOSITORY_H

#include <iostream>
#include "idgenerator.h"

class UserRequestsRepository
{
private:
    IdGenerator _generator;
    char _waiterRequestId[33];
    char _billRequestId[33];
    void CancelWaiterRequest() { _waiterRequestId[0] = 0; }
    void CancelBillRequest() { _billRequestId[0] = 0; }
public:
    UserRequestsRepository();
    const char *SetWaiterIsRequested();
    const char *SetBillIsRequested();
    void CancelAllRequest();
    bool CancelRequest(const char *requestId);
    bool IsWaiterRequested() { return _waiterRequestId[0] != 0; }
    bool IsBillRequested() { return _billRequestId[0] != 0; }
};

UserRequestsRepository::UserRequestsRepository()
{
    CancelAllRequest();
}

const char *UserRequestsRepository::SetWaiterIsRequested()
{
    if (IsWaiterRequested() == false)
    {
        strcpy(_waiterRequestId, _generator.Generate());
    }
    return _waiterRequestId;
}

const char *UserRequestsRepository::SetBillIsRequested()
{
    if (IsBillRequested() == false)
    {
        strcpy(_billRequestId, _generator.Generate());
    }
    return _billRequestId;
}

bool UserRequestsRepository::CancelRequest(const char *requestId)
{
    if (strcmp(_waiterRequestId, requestId) == 0)
    {
        CancelWaiterRequest();
        return true;
    }
    if (strcmp(_billRequestId, requestId) == 0)
    {
        CancelBillRequest();
        return true;
    }
    return false;
}

void UserRequestsRepository::CancelAllRequest()
{
    CancelWaiterRequest();
    CancelBillRequest();
}

#endif
