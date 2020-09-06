#ifndef USERREQUESTSREPOSITORY_H
#define USERREQUESTSREPOSITORY_H

#include <iostream>
#include "interfaces\icommunicator.h"
#include "idgenerator.h"

class UserRequestsRepository
{
private:
    ICommunicator *_communicator = nullptr;
    IdGenerator _generator;
    char _waiterRequestId[33];
    char _billRequestId[33];
    void CancelWaiterRequest() { _waiterRequestId[0] = 0; }
    void CancelBillRequest() { _billRequestId[0] = 0; }
public:
    UserRequestsRepository(ICommunicator *communicator);
    const char *SetWaiterIsRequested();
    const char *SetBillIsRequested();
    void CancelAllRequest();
    bool CancelRequest(const char *requestId);
    bool IsWaiterRequested() { return _waiterRequestId[0] != 0; }
    bool IsBillRequested() { return _billRequestId[0] != 0; }
};

UserRequestsRepository::UserRequestsRepository(ICommunicator *communicator)
{
    _communicator = communicator;
    CancelAllRequest();
}

const char *UserRequestsRepository::SetWaiterIsRequested()
{
    if (IsWaiterRequested() == false)
    {
        strcpy(_waiterRequestId, _generator.Generate());
        _communicator->RequestWaiter(_waiterRequestId);
    }
    return _waiterRequestId;
}

const char *UserRequestsRepository::SetBillIsRequested()
{
    if (IsBillRequested() == false)
    {
        strcpy(_billRequestId, _generator.Generate());
        _communicator->RequestBill(_billRequestId);
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
