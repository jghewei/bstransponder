#include "Snmp.h"
#include "CThread.h"
#include "InventoryMob.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/mib_modules.h>

void SnmpTrapTest();

bool bSnmpRunning = true;

oid vendorOid = 19293;

extern const oid snmptrap_oid[];
extern const size_t snmptrap_oid_len;

void* SnmpServiceThread(void *param)
{
    const char *app_name = "snmpd";
    
    system("mkdir -p /tmp/snmp; echo \"rocommunity public\" > /tmp/snmp/snmpd.conf; echo \"rwcommunity private\" >> /tmp/snmp/snmpd.conf;");

    /* we are a subagent */  
    //netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);

    // disable internal notification logging
    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_DONT_RETAIN_NOTIFICATIONS, 1);

    // not supporting AgentX
    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_AGENTX_MASTER, 0);

    //netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_VERBOSE, 1);
    
    set_configuration_directory("/tmp/snmp");

    /* initialize the agent library */
    init_agent(app_name);

    init_mib_modules();

    /* initialize your mib code here */
    InitMibs();

    /* Test will be used to read Test.conf files. */
    init_snmp(app_name);

    init_master_agent();

    //SnmpTrapTest();

    while (bSnmpRunning)
    {  
        agent_check_and_process(1);/* block every 1 second */
    }

    /* at shutdown time */
    snmp_shutdown(app_name);

    shutdown_master_agent();

    /* deinitialize your mib code here */

    /* shutdown the agent library */
    shutdown_agent();

    return NULL;
}

int SnmpAgentInit(void)
{
    InventoryMob *pMob = InventoryMob::GetInst(1);
    if (pMob != NULL && pMob->VendorName.Get() == "CNMP")
    {
        vendorOid = 16589;
        APP_LOG("change vendor oid to %d", (int)vendorOid);
    }

    new CThread("snmp", SnmpServiceThread, NULL);
    return 0;
}

void SnmpAgentExit(void)
{
    bSnmpRunning = false;
}

int SnmpGetRequestInstance(void *request)
{
    int inst = 0;
    netsnmp_request_info *req = (netsnmp_request_info*)request;

    if (req == NULL) return 0;
    if (req->requestvb == NULL) return 0;

#if 1
    printf("OID: ");
    for (size_t i = 0; i < req->requestvb->name_length; i++)
    {
        printf("%d.", (int)(req->requestvb->name[i]));
    }
    printf("\n");
#endif

    inst = (int)(req->requestvb->name[req->requestvb->name_length - 1]);

    return inst;
}

void SnmpShowRequestOid(void *request)
{
    netsnmp_request_info *req = (netsnmp_request_info*)request;

    if (req == NULL) return;
    if (req->requestvb == NULL) return;

    printf("OID: ");
    for (size_t i = 0; i < req->requestvb->name_length; i++)
    {
        printf("%d.", (int)(req->requestvb->name[i]));
    }
    printf("\n");
}

void SnmpTrapAddVar(netsnmp_variable_list **var_list, int index, const char *value)
{
    oid alarm_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 100, 1, 1, 0 };
    alarm_oid[OID_LENGTH(alarm_oid)-1] = index;
    snmp_varlist_add_variable(var_list,
                               alarm_oid, OID_LENGTH(alarm_oid),
                               ASN_OCTET_STR,
                              (u_char *)value, strlen(value));
}

void SnmpTrapTest()
{
    SnmpCreateTrapSession("192.168.31.169:1622", NULL, NULL);
    SnmpSendTrap("Eth-1", "LOS", "SA", "Critical", "2018/05/05", "LOS defect", "raise");
}

bool SnmpSinkParser(const char *trap, std::string &ip, unsigned short &port)
{
    if (trap == NULL) return false;
    
    char *colon = strchr((char*)trap, ':');
    if (colon == NULL) return false;

    if (strlen(colon + 1) >= 1)
    {
        int val = atoi(colon + 1);
        if (val > 0 && val < 65535)
        {
            port = (unsigned short)val;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    if ((colon - trap) >= 7)
    {
        ip.assign((char*)trap, colon - trap);
    }
    else
    {
        return false;
    }

    APP_LOG("Trap Sink ip: %s, port: %d", ip.c_str(), (int)port);

    return true;
}

static int gTrapSession = 0;

void SnmpCreateTrapSession(const char *trap1, const char *trap2, const char *trap3)
{
    snmpd_free_trapsinks();

    std::string ip;
    unsigned short port;

    gTrapSession = 0;

    if (SnmpSinkParser(trap1, ip, port))
    {
        create_trap_session((char*)ip.c_str(), port, "public", SNMP_VERSION_2c, SNMP_MSG_TRAP2);
        gTrapSession++;
    }

    if (SnmpSinkParser(trap2, ip, port))
    {
        create_trap_session((char*)ip.c_str(), port, "public", SNMP_VERSION_2c, SNMP_MSG_TRAP2);
        gTrapSession++;
    }

    if (SnmpSinkParser(trap3, ip, port))
    {
        create_trap_session((char*)ip.c_str(), port, "public", SNMP_VERSION_2c, SNMP_MSG_TRAP2);
        gTrapSession++;
    }
}

void SnmpSendTrap(const char *key, const char *name, const char *type,
                  const char *severity, const char *time, const char *desp,
                  const char *action)
{
    if (gTrapSession == 0) return;
    
    netsnmp_variable_list* var_list = NULL;

    oid alarm_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, 200, 1 };
    snmp_varlist_add_variable(&var_list,
                              snmptrap_oid, snmptrap_oid_len,
                              ASN_OBJECT_ID,
                              (u_char *) alarm_oid,
                              OID_LENGTH(alarm_oid) * sizeof(oid));

    SnmpTrapAddVar(&var_list, 1, key);
    SnmpTrapAddVar(&var_list, 2, name);
    SnmpTrapAddVar(&var_list, 3, type);
    SnmpTrapAddVar(&var_list, 4, severity);
    SnmpTrapAddVar(&var_list, 5, time);
    SnmpTrapAddVar(&var_list, 6, desp);
    SnmpTrapAddVar(&var_list, 7, action);

    if(var_list != NULL)
    {
        send_v2trap(var_list);
        APP_LOG("send_v2trap");
        snmp_free_varbind(var_list);
    }
}

