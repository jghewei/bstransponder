#ifndef _SNMP_H_
#define _SNMP_H_

#include "DanOS.h"
#include <net-snmp/library/oid.h>

extern oid vendorOid;

void InitMibs(void);

int SnmpAgentInit(void);
void SnmpAgentExit(void);

void SnmpCreateTrapSession(const char *trap1, const char *trap2, const char *trap3);
void SnmpSendTrap(const char *key, const char *name, const char *type,
                  const char *severity, const char *time, const char *desp,
                  const char *action);


// private
int SnmpGetRequestInstance(void *request);
void SnmpShowRequestOid(void *request);

#endif
