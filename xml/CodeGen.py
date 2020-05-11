#!/usr/bin/python
# -*- coding:utf-8 -*-
# This script is based on python 3.x and can run on python 3.4 and python 2.7
# It will read a database.xml file and generate database related files

from xml.etree import ElementTree

mibFileName = "DANRIVER-MXP-MIB.mib"
mibHeadFile = "MibHeader.mib"
mibFootFile = "MibFooter.mib"
mibIndex = 1

vendorList = [
    {'name' : 'DanRiver', 'product' : 'CTX2000', 'oid' : '19293'},
    {'name' : 'CNMP', 'product' : 'eWAVE5110', 'oid' : '16589'},
]

class AtrbObj(object):
    def __init__(self, name, type):
        self.name = name
        self.type = type
        self.readonly = False
        self.persistent = False
        self.tableshow = True

    def __str__(self):
        #return '[' + self.name + ', ' + self.type + ', ' + self.readonly + ', ' + self.persistent
        return "aaaa"

class EnumItemObj(object):
    def __init__(self, name, value):
        self.name = name
        self.value = value

class AlarmObj(object):
    def __init__(self, name, sa, severity, desp):
        self.name = name
        self.sa = sa
        self.severity = severity
        self.desp = desp

class CounterObj(object):
    def __init__(self, name, unit):
        self.name = name
        self.unit = unit

def gen_mob_h(mob_name, enum_list, atrb_list, alarm_list, counter_list):
    mob_name_upper = mob_name.upper()
    filename = '../DBF/inc/' + mob_name + '.h'
    f = open(filename, 'w')
    f.write('#ifndef _' + mob_name_upper + '_H_\n')
    f.write('#define _' + mob_name_upper + '_H_\n')
    f.write('\n')
    f.write('#include <vector>\n')
    f.write('#include "Key.h"\n')
    f.write('#include "Attribute.h"\n')
    f.write('#include "Alarm.h"\n')
    f.write('\n')
    for enum in enum_list:
        prefix = 'const int ' + mob_name + '_' + enum[0] + '_'
        for item in enum[1]:
            f.write(prefix + item.name + ' = ' + item.value + ';\n')
    f.write('\n')
    for enum in enum_list:
        f.write('const char* ' + mob_name + '_' + enum[0] + '_to_string(int value);\n')
    f.write('\n')
    if len(alarm_list) > 0:
        alm_name = mob_name
        alm_name = alm_name.replace('Mob', 'Alm')
        f.write('// AID - alarm id\n')
        f.write('const char* ' + alm_name + '_desp_string(AlarmId::Enum aid);\n')
        f.write('\n')
    if len(counter_list) > 0:
        cnt_id = 0
        cnt_name = mob_name
        cnt_name = cnt_name.replace('Mob', 'Cnt')
        f.write('// CID - Counter id\n')
        for cnt in counter_list:
            f.write('const int ' + cnt_name + '_' + cnt.name + ' = ' + str(cnt_id) + ';\n')
            cnt_id = cnt_id + 1
        f.write('\n')
        f.write('const char* ' + cnt_name + '_name_string(int cid);\n')
        f.write('const char* ' + cnt_name + '_unit_string(int cid);\n')
        f.write('\n')
    f.write('class ' + mob_name + ' : public BaseMob\n')
    f.write('{\n')
    f.write('public:\n')
    f.write('    static ' + mob_name + '* AddInst(KeyType::Enum type, int id);\n')
    f.write('    static ' + mob_name + '* GetInst(int inst);\n')
    f.write('    static ' + mob_name + '* GetInstByKey(Key &key);\n')
    f.write('    static ' + mob_name + '* GetInstByKey(KeyType::Enum type, int id);\n')
    f.write('    static int GetInstNum();\n')
    f.write('    static void RegSetCallback(onSetCallback cb) { mSetCb = cb; }\n')
    f.write('    static void RegGetCallback(onGetCallback cb) { mGetCb = cb; }\n')
    f.write('    static bool SetCallback(BaseMob* mob, std::string& atrb) { if (mSetCb == NULL) return false; return (*mSetCb)(mob, atrb); }\n')
    f.write('    static bool GetCallback(BaseMob* mob, std::string& atrb) { if (mGetCb == NULL) return false; return (*mGetCb)(mob, atrb); }\n')
    f.write('\n')
    f.write('    ' + mob_name + '(KeyType::Enum type, int id);\n')
    f.write('    void Save();\n')
    f.write('    void Load();\n')
    f.write('    void UpdatePM() { std::string s = "Counter"; GetCallback(this, s); }\n')
    f.write('    std::string GetKeyName() { return key.name(); }\n')
    f.write('\n')
    f.write('public:\n')
    for atrb in atrb_list:
        atrb_type = atrb.type
        if atrb_type == 'string':
            atrb_type = 'std::string'
        if atrb_type == 'enum':
            atrb_type = 'int'
        f.write('    Attribute<' + atrb_type + ', ' + mob_name + '> ' + atrb.name + ';\n')
    f.write('\n')
    f.write('    Key key;\n')
    f.write('\n')
    f.write('private:\n')
    f.write('    static std::vector<' + mob_name + '*> mInst;\n')
    f.write('    static int mInstNum;\n')
    f.write('    static onSetCallback mSetCb;\n')
    f.write('    static onGetCallback mGetCb;\n')
    f.write('};\n')
    f.write('\n')
    f.write('#endif\n')
    f.write('\n')
    f.close()

def gen_mob_cpp(mob_name, enum_list, atrb_list, alarm_list, counter_list):
    filename = '../DBF/src/' + mob_name + '.cpp'
    f = open(filename, 'w')
    f.write('#include "' + mob_name + '.h"\n')
    f.write('\n')
    f.write('std::vector<' + mob_name + '*> ' + mob_name + '::mInst;\n')
    f.write('int ' + mob_name + '::mInstNum = 0;\n')
    f.write('onSetCallback ' + mob_name + '::mSetCb = NULL;\n')
    f.write('onGetCallback ' + mob_name + '::mGetCb = NULL;\n')
    f.write('#ifdef __TARGET__\n')
    f.write('static const char *DBFile = "/mnt/sysfs/' + mob_name + '.";\n')
    f.write('#else\n')
    f.write('static const char *DBFile = "/tmp/' + mob_name + '.";\n')
    f.write('#endif\n')
    f.write('\n')
    f.write('' + mob_name + '* ' + mob_name + '::AddInst(KeyType::Enum type, int id)\n')
    f.write('{\n')
    f.write('    ' + mob_name + ' *inst = new ' + mob_name + '(type, id);\n')
    f.write('    mInst.push_back(inst);\n')
    f.write('    mInstNum++;\n')
    f.write('    return inst;\n')
    f.write('}\n')
    f.write('\n')
    f.write('' + mob_name + '* ' + mob_name + '::GetInst(int inst)\n')
    f.write('{\n')
    f.write('    if (inst < GetInstNum())\n')
    f.write('    {\n')
    f.write('        return mInst[inst];\n')
    f.write('    }\n')
    f.write('    else\n')
    f.write('    {\n')
    f.write('        return NULL;\n')
    f.write('    }\n')
    f.write('}\n')
    f.write('\n')
    f.write('' + mob_name + '* ' + mob_name + '::GetInstByKey(Key &key)\n')
    f.write('{\n')
    f.write('    for (int i = 0; i < GetInstNum(); i++)\n')
    f.write('    {\n')
    f.write('        if (mInst[i]->key == key)\n')
    f.write('        {\n')
    f.write('            return mInst[i];\n')
    f.write('        }\n')
    f.write('    }\n')
    f.write('    return NULL;\n')
    f.write('}\n')
    f.write('\n')
    f.write('' + mob_name + '* ' + mob_name + '::GetInstByKey(KeyType::Enum type, int id)\n')
    f.write('{\n')
    f.write('    Key key(type, id);\n')
    f.write('    return GetInstByKey(key);\n')
    f.write('}\n')
    f.write('\n')
    f.write('int ' + mob_name + '::GetInstNum()\n')
    f.write('{\n')
    f.write('    return mInstNum;\n')
    f.write('}\n')
    f.write('\n')
    f.write('' + mob_name + '::' + mob_name + '(KeyType::Enum type, int id) :\n')
    for atrb in atrb_list:
        f.write('    ' + atrb.name + '("' + atrb.name + '", this),\n')
    f.write('    key(type, id)\n')
    f.write('{\n')
    alm_name = mob_name
    alm_name = alm_name.replace('Mob', 'Alm')
    for alm in alarm_list:
        aid = 'AlarmId::' + alm.name
        aid_desp = alm_name + '_desp_string(' + aid + ')'
        f.write('    AddAlarm(' + aid + ', new Alarm(key, ' + aid + ', ' + aid_desp + ', ' + alm.sa + ', AlarmSeverity::ALM_' + alm.severity + '));\n')
    cnt_name = mob_name
    cnt_name = cnt_name.replace('Mob', 'Cnt')
    for cnt in counter_list:
        cid = cnt_name + '_' + cnt.name
        cid_name = cnt_name + '_name_string(' + cid + ')'
        cid_unit = cnt_name + '_unit_string(' + cid + ')'
        f.write('    AddCounter(' + cid + ', new Counter(key, ' + cid + ', ' + cid_name + ', ' + cid_unit + '));\n')
    f.write('}\n')
    f.write('\n')
    f.write('void ' + mob_name + '::Save()\n')
    f.write('{\n')
    f.write('    std::string buffer;\n')
    f.write('\n')
    for atrb in atrb_list:
        if atrb.persistent == True:
            f.write('    buffer += "' + atrb.name + '=";\n')
            if atrb.type == 'string':
                f.write('    buffer += ' + atrb.name + '.GetValue();\n')
            else:
                f.write('    buffer += to_string(' + atrb.name + '.GetValue());\n')
            f.write('    buffer += "\\n";\n')
            f.write('\n')
    f.write('    if (buffer.size() == 0) return;\n')
    f.write('    APP_LOG("%s", buffer.c_str());\n')
    f.write('    std::string fname = DBFile;\n')
    f.write('    fname += to_string(key.id());\n')
    f.write('    FILE *fp = fopen(fname.c_str(), "w");\n')
    f.write('    if (fp == NULL) return;\n')
    f.write('    fwrite(buffer.data(), 1, buffer.size(), fp);\n')
    f.write('    fclose(fp);\n')            
    f.write('}\n')
    f.write('\n')
    f.write('void ' + mob_name + '::Load()\n')
    f.write('{\n')
    f.write('    std::string fname = DBFile;\n')
    f.write('    fname += to_string(key.id());\n')
    f.write('    FILE *fp = fopen(fname.c_str(), "r");\n')
    f.write('    if (fp == NULL) return;\n')
    f.write('    char linebuf[256] = {0};\n')
    f.write('\n')
    f.write('    while (fgets(linebuf, 256, fp) != NULL)\n')
    f.write('    {\n')
    f.write('        int len = strlen(linebuf);\n')
    f.write('        if (len < 3) continue;\n')
    f.write('        if (linebuf[len-1] == \'\\n\') linebuf[len-1] = 0;\n')
    f.write('\n')
    f.write('        if (0) {}\n')
    for atrb in atrb_list:
        if atrb.persistent == True:
            f.write('        else if (check_cfg_name(' + atrb.name + '.name(), linebuf))\n')
            if atrb.type == 'string':
                f.write('            ' + atrb.name + '.SetValue((const char*)&linebuf[' + atrb.name + '.name().size()+1]);\n')
            elif atrb.type == 'float':
                f.write('            ' + atrb.name + '.SetValue(atof((const char*)&linebuf[' + atrb.name + '.name().size()+1]));\n')
            else:
                f.write('            ' + atrb.name + '.SetValue(atoi((const char*)&linebuf[' + atrb.name + '.name().size()+1]));\n')
    f.write('    }\n')
    f.write('\n')
    f.write('    fclose(fp);\n')
    f.write('}\n')
    f.write('\n')
    for enum in enum_list:
        f.write('const char* ' + mob_name + '_' + enum[0] + '_to_string(int value)\n')
        f.write('{\n')
        f.write('    switch (value) {\n')
        for item in enum[1]:
            f.write('        case ' + item.value + ': return "' + item.name + '";\n')
        f.write('        default: return "???";\n')
        f.write('    }\n')
        f.write('}\n')
        f.write('\n')
    if len(alarm_list) > 0:
        f.write('const char* ' + alm_name + '_desp_string(AlarmId::Enum aid)\n')
        f.write('{\n')
        f.write('    switch (aid) {\n')
        for alm in alarm_list:
            aid = 'AlarmId::' + alm.name
            f.write('        case ' + aid + ': return "' + alm.desp + '";\n')
        f.write('        default: return "???";\n')
        f.write('    }\n')
        f.write('}\n')
        f.write('\n')
    if len(counter_list) > 0:
        f.write('const char* ' + cnt_name + '_name_string(int cid)\n')
        f.write('{\n')
        f.write('    switch (cid) {\n')
        for cnt in counter_list:
            cid = cnt_name + '_' + cnt.name
            f.write('        case ' + cid + ': return "' + cnt.name + '";\n')
        f.write('        default: return "???";\n')
        f.write('    }\n')
        f.write('}\n')
        f.write('\n')
        f.write('const char* ' + cnt_name + '_unit_string(int cid)\n')
        f.write('{\n')
        f.write('    switch (cid) {\n')
        for cnt in counter_list:
            cid = cnt_name + '_' + cnt.name
            f.write('        case ' + cid + ': return "' + cnt.unit + '";\n')
        f.write('        default: return "???";\n')
        f.write('    }\n')
        f.write('}\n')
        f.write('\n')
    f.close()

def gen_page_cpp(mname, enum_list, atrb_list):
    filename = '../WEB/src/' + mname + 'Page.cpp'
    f = open(filename, 'w')
    f.write('#include "DanOS.h"\n')
    f.write('#include "' + mname + 'Mob.h"\n')
    f.write('#include "Web.h"\n')
    f.write('\n')
    f.write('void Web' + mname + 'Get(Webs *wp)\n')
    f.write('{\n')
    f.write('    int inst = 0, num = ' + mname + 'Mob::GetInstNum();\n')
    f.write('    char *pid = websGetVar(wp, "id", "");\n')
    f.write('    if (strlen(pid) > 0)\n')
    f.write('    {\n')
    f.write('        inst = atoi(pid);\n')
    f.write('        num = inst + 1;\n')
    f.write('    }\n')
    f.write('\n')
    f.write('    CJson webs;\n')
    f.write('    for (; inst < num; inst++)\n')
    f.write('    {\n')
    f.write('        ' + mname + 'Mob *pMob = ' + mname + 'Mob::GetInst(inst);\n')
    f.write('        if (pMob == NULL) continue;\n')
    f.write('\n')
    f.write('        webs.AddEntry();\n')
    if mname == "Network": # for Network mob, to hide inst number in Web page
        f.write('        webs.AddValue("Key", pMob->key.name().substr(0, pMob->key.name().size() - 2));\n')
    else:
        f.write('        webs.AddValue("Key", pMob->key.name());\n')
    for atrb in atrb_list:
        f.write('        webs.AddValue("' + atrb.name + '", pMob->' + atrb.name + '.Get());\n')
    f.write('\n')
    f.write('        webs.AddValue("id", to_string(inst));\n')
    f.write('        webs.EndEntry();\n')
    f.write('    }\n')
    f.write('    webs.End();\n')
    f.write('\n')
    f.write('    websSetStatus(wp, 200);\n')
    f.write('    websWriteHeaders(wp, webs.GetLength(), 0);\n')
    f.write('    websWriteEndHeaders(wp);\n')
    f.write('    websWriteBlock(wp, (char*)webs.GetBuffer(), webs.GetLength());\n')
    f.write('    websDone(wp);\n')
    f.write('}\n')
    f.write('\n')
    f.write('void Web' + mname + 'Set(Webs *wp)\n')
    f.write('{\n')
    f.write('    int inst = 0;\n')
    f.write('    char *ptr = NULL;\n')
    f.write('    std::string resp = WebFormResponse(true, "command complete");\n')
    f.write('\n')
    f.write('    ptr = websGetVar(wp, "id", "");\n')
    f.write('    if (strlen(ptr) > 0)\n')
    f.write('    {\n')
    f.write('        inst = atoi(ptr);\n')
    f.write('    }\n')
    f.write('\n')
    f.write('    ' + mname + 'Mob *pMob = ' + mname + 'Mob::GetInst(inst);\n')
    f.write('    if (pMob == NULL) goto error_exit;\n')
    f.write('\n')
    for atrb in atrb_list:
        if atrb.readonly == True:
            continue
        f.write('    ptr = websGetVar(wp, "' + atrb.name + '", "");\n')
        f.write('    if (strlen(ptr) > 0)\n')
        f.write('    {\n')
        if atrb.type == 'enum' or atrb.type == 'int':
            f.write('        if (!pMob->' + atrb.name + '.Set(atoi(ptr))) goto error_exit;\n')
        elif atrb.type == 'float':
            f.write('        if (!pMob->' + atrb.name + '.Set(atof(ptr))) goto error_exit;\n')
        else:
            f.write('        if (!pMob->' + atrb.name + '.Set(ptr)) goto error_exit;\n')
        f.write('    }\n')
        f.write('\n')
    f.write('    pMob->Save();\n')
    f.write('    goto set_end;\n')
    f.write('\n')
    f.write('error_exit:\n')
    f.write('    resp = WebFormResponse(true, "command failed");\n')
    f.write('\n')
    f.write('set_end:\n')
    f.write('    websSetStatus(wp, 200);\n')
    f.write('    websWriteHeaders(wp, resp.size(), 0);\n')
    f.write('    websWriteEndHeaders(wp);\n')
    f.write('    websWrite(wp, (char*)resp.c_str());\n')
    f.write('    websDone(wp);\n')
    f.write('}\n')
    f.write('\n')
    f.write('void Web' + mname + 'Action(Webs *wp)\n')
    f.write('{\n')
    f.write('    if (isWebPostMethod(wp))\n')
    f.write('        Web' + mname + 'Set(wp);\n')
    f.write('    else\n')
    f.write('        Web' + mname + 'Get(wp);\n')
    f.write('}\n')
    f.write('\n')
    f.close()

def gen_snmp_cpp(mname, enum_list, atrb_list):
    global mibIndex
    entryIndex = 1
    filename = '../SNMP/src/' + mname + 'Mib.cpp'
    f = open(filename, 'w')
    f.write('#include "DanOS.h"\n')
    f.write('#include "Snmp.h"\n')
    f.write('#include "' + mname + 'Mob.h"\n')
    f.write('#include <net-snmp/net-snmp-config.h>\n')
    f.write('#include <net-snmp/net-snmp-includes.h>\n')
    f.write('#include <net-snmp/agent/net-snmp-agent-includes.h>\n')
    f.write('\n')
    for atrb in atrb_list:
        data_type = "ASN_INTEGER"
        if atrb.type == 'string':
            data_type = "ASN_OCTET_STR"
        f.write('static int handle_' + atrb.name + '(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)\n')
        f.write('{\n')
        f.write('    ' + mname + 'Mob *pMob = ' + mname + 'Mob::GetInst(0);\n')
        f.write('    if (pMob == NULL) return SNMP_ERR_GENERR;\n')
        f.write('\n')
        f.write('    switch (reqinfo->mode)\n')
        f.write('    {\n')
        f.write('        case MODE_GET:\n')
#        f.write('            APP_LOG_LINE();\n')
        if atrb.type == 'string':
            f.write('            snmp_set_var_typed_value(requests->requestvb, ' + data_type + ', pMob->' + atrb.name + '.Get().c_str(), pMob->' + atrb.name + '.Get().size());\n')
        else:
            f.write('            snmp_set_var_typed_value(requests->requestvb, ' + data_type + ', &(pMob->' + atrb.name + '.Get()), sizeof(pMob->' + atrb.name + '.Get()));\n')
        f.write('            break;\n')
        f.write('\n')
        f.write('        case MODE_SET_ACTION:\n')
        if atrb.type == 'string':
            f.write('            pMob->' + atrb.name + '.Set((char*)requests->requestvb->buf);\n')
        else:
            #f.write('            if (requests->requestvb->val_len == sizeof(int))\n')
            f.write('            {\n')
            f.write('                int val = 0;\n')
            f.write('                memcpy(&val, requests->requestvb->buf, sizeof(int));\n')
            f.write('                pMob->' + atrb.name + '.Set(val);\n')
            f.write('            }\n')
        #f.write('            if (0 /* XXX: error? */ ) {\n')
        #f.write('                netsnmp_set_request_error(reqinfo, requests, 0/* some error */);\n')
        #f.write('            }\n')
        f.write('            break;\n')
        f.write('\n')
        f.write('        default:\n')
        f.write('            break;\n')
        f.write('    }\n')
        f.write('\n')
        f.write('    return SNMP_ERR_NOERROR;\n')
        f.write('}\n')
        f.write('\n')
    f.write('void Init' + mname + 'Mib(void)\n')
    f.write('{\n')
    for atrb in atrb_list:
        rwCtrl = 'HANDLER_CAN_RWRITE'
        if atrb.readonly == True:
            rwCtrl = 'HANDLER_CAN_RONLY'
        f.write('    static oid ' + atrb.name + '_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, ' + str(mibIndex) + ', ' + str(entryIndex) + ' };\n')
        f.write('    netsnmp_register_scalar(netsnmp_create_handler_registration("' + atrb.name + '", handle_' + atrb.name + ', ' + atrb.name + '_oid, OID_LENGTH(' + atrb.name + '_oid), ' + rwCtrl + '));\n')
        f.write('\n')
        entryIndex = entryIndex + 1
    f.write('}\n')
    f.close()

def gen_snmpTable_cpp(mname, enum_list, atrb_list):
    global mibIndex
    filename = '../SNMP/src/' + mname + 'Mib.cpp'
    f = open(filename, 'w')
    f.write('#include "DanOS.h"\n')
    f.write('#include "Snmp.h"\n')
    f.write('#include "' + mname + 'Mob.h"\n')
    f.write('#include <net-snmp/net-snmp-config.h>\n')
    f.write('#include <net-snmp/net-snmp-includes.h>\n')
    f.write('#include <net-snmp/agent/net-snmp-agent-includes.h>\n')
    f.write('\n')
    f.write('static netsnmp_table_data_set *table_set = NULL;\n')
    f.write('\n')
    f.write('static int handle_' + mname + '(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)\n')
    f.write('{\n')
    f.write('    int inst = SnmpGetRequestInstance(requests);\n')
    f.write('    ' + mname + 'Mob *pMob = ' + mname + 'Mob::GetInst(inst);\n')
    f.write('    if (pMob == NULL) return SNMP_ERR_GENERR;\n')
    f.write('\n')
    f.write('    netsnmp_table_request_info *table_info = netsnmp_extract_table_info(requests);\n')
    f.write('\n')
    f.write('    switch (reqinfo->mode)\n')
    f.write('    {\n')
    f.write('        case MODE_GET:\n')
    #f.write('        case MODE_GETNEXT:\n')
    f.write('            switch (table_info->colnum)\n')
    f.write('            {\n')
    f.write('                case 1:\n')
    f.write('                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->key.name().c_str(), pMob->key.name().size());\n')
    f.write('                    break;\n')
    entryIndex = 2
    for atrb in atrb_list:
        f.write('                case ' + str(entryIndex) + ':\n')
        if atrb.type == 'string':
            f.write('                    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, pMob->' + atrb.name + '.Get().c_str(), pMob->' + atrb.name + '.Get().size());\n')
        elif atrb.type == 'float':
            f.write('                    {\n')
            f.write('                        char fval[32] = {0};\n')
            f.write('                        sprintf(fval, "%.2f", pMob->' + atrb.name + '.Get());\n')
            f.write('                        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, fval, strlen(fval));\n')
            f.write('                    }\n')
        else:
            f.write('                    snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &(pMob->' + atrb.name + '.Get()), sizeof(pMob->' + atrb.name + '.Get()));\n')
        f.write('                    break;\n')
        entryIndex = entryIndex + 1
    f.write('                default:\n')
    f.write('                    break;\n')
    f.write('            }\n')
    f.write('            break;\n')
    f.write('\n')
    f.write('        case MODE_SET_ACTION:\n')
    f.write('            switch (table_info->colnum)\n')
    f.write('            {\n')
    entryIndex = 2
    for atrb in atrb_list:
        if atrb.readonly != True:
            f.write('                case ' + str(entryIndex) + ':\n')
            if atrb.type == 'string':
                f.write('                    pMob->' + atrb.name + '.Set((char*)requests->requestvb->buf);\n')
            else:
                #f.write('                    if (requests->requestvb->val_len == sizeof(int))\n')
                f.write('                    {\n')
                f.write('                        int val = 0;\n')
                f.write('                        memcpy(&val, requests->requestvb->buf, sizeof(int));\n')
                f.write('                        pMob->' + atrb.name + '.Set(val);\n')
                f.write('                    }\n')
            #f.write('                    if (0 /* XXX: error? */ ) {\n')
            #f.write('                        netsnmp_set_request_error(reqinfo, requests, 0/* some error */);\n')
            #f.write('                    }\n')
            f.write('                    break;\n')
        entryIndex = entryIndex + 1
    f.write('                default:\n')
    f.write('                    break;\n')
    f.write('            }\n')
    f.write('            break;\n')
    f.write('\n')
    f.write('        default:\n')
    f.write('            break;\n')
    f.write('    }\n')
    f.write('\n')
    f.write('    return SNMP_ERR_NOERROR;\n')
    f.write('}\n')
    f.write('\n')
    f.write('void Init' + mname + 'Mib(void)\n')
    f.write('{\n')
    f.write('    static oid ' + mname + '_oid[] = { 1, 3, 6, 1, 4, 1, vendorOid, 2, ' + str(mibIndex) + ', 1 };\n')
    f.write('    table_set = netsnmp_create_table_data_set("' + mname + '");\n')
    f.write('    table_set->allow_creation = 1;\n')
    f.write('    netsnmp_table_set_add_indexes(table_set, ASN_INTEGER, 0);\n')
    f.write('    netsnmp_table_set_multi_add_default_row(table_set,\n')
    f.write('                                            1, ASN_OCTET_STR, 0, NULL, 0,\n');
    entryIndex = 2
    for atrb in atrb_list:
        f.write('                                            ')
        f.write(str(entryIndex) + ', ')
        if atrb.type == 'string':
            f.write('ASN_OCTET_STR, ')
        else:
            f.write('ASN_INTEGER, ')
        if atrb.readonly == True:
            f.write('0, ')
        else:
            f.write('1, ')
        f.write('NULL, 0,\n')
        entryIndex = entryIndex + 1
    f.write('                                            0);\n');
    f.write('    netsnmp_register_table_data_set(netsnmp_create_handler_registration("' + mname + '", handle_' + mname + ', ' + mname + '_oid, OID_LENGTH(' + mname + '_oid), HANDLER_CAN_RWRITE), table_set, NULL);\n')
    f.write('\n');
    f.write('    {\n');
    f.write('        int inst = 0, num = ' + mname + 'Mob::GetInstNum();\n')
    f.write('        for (; inst < num; inst++)\n')
    f.write('        {\n')
    f.write('            ' + mname + 'Mob *pMob = ' + mname + 'Mob::GetInst(inst);\n')
    f.write('            if (pMob == NULL) continue;\n')
    f.write('\n')
    f.write('            netsnmp_table_row *prow = netsnmp_create_table_data_row();\n')
    f.write('            netsnmp_table_row_add_index(prow, ASN_INTEGER, &inst, sizeof(inst));\n')
    f.write('\n')
    f.write('            netsnmp_set_row_column(prow, 1, ASN_OCTET_STR, pMob->key.name().c_str(), pMob->key.name().size());\n')
    f.write('            netsnmp_mark_row_column_writable(prow, 1, 0);\n')
    entryIndex = 2
    for atrb in atrb_list:
        write_able = 1
        if atrb.readonly == True:
            write_able = 0
        if atrb.type == 'string':
            f.write('            netsnmp_set_row_column(prow, ' + str(entryIndex) + ', ASN_OCTET_STR, pMob->' + atrb.name + '.Get().c_str(), pMob->' + atrb.name + '.Get().size());\n')
        else:
            f.write('            netsnmp_set_row_column(prow, ' + str(entryIndex) + ', ASN_INTEGER, &pMob->' + atrb.name + '.Get(), sizeof(pMob->' + atrb.name + '.Get()));\n')
        f.write('            netsnmp_mark_row_column_writable(prow, ' + str(entryIndex) + ', ' + str(write_able) + ');\n')
        entryIndex = entryIndex + 1
    f.write('\n')
    f.write('            netsnmp_table_dataset_add_row(table_set, prow);\n')
    f.write('        }\n')
    f.write('    }\n')
    f.write('}\n')
    f.write('\n')
    f.close()

def gen_table_html(mname, enum_list, atrb_list):
    filename = '../html/tpl/' + mname + 'Table.html'
    f = open(filename, 'w')
    f.write('<script>\n')
    f.write('\'use strict\';\n')
    f.write('app.controller(\'TableCtrl\', [\'$scope\', \'$http\', function($scope, $http) {\n')
    f.write('    $http.get("/action/' + mname + '")\n')
    f.write('    .success(function (response) {$scope.aaData = response.aaData;});\n')
    f.write('\n')
    for enum in enum_list:
        f.write('    $scope.' + enum[0] + 'Enum = {')
        for item in enum[1]:
            f.write(item.value + ':"' + item.name + '",')
        f.write('99:"INVALID"')
        f.write('};\n');
    f.write('\n')
    f.write('}]);\n')
    f.write('</script>\n')
    f.write('\n')
    f.write('<div class="bg-light lter b-b wrapper-md">\n')
    f.write('  <h1 class="m-n font-thin h3">' + mname + ' Table</h1>\n')
    f.write('</div>\n')
    f.write('<div class="wrapper-md">\n')
    f.write('  <div class="panel panel-default">\n')
    f.write('    <div class="table-responsive" ng-controller="TableCtrl">\n')
    f.write('      <table class="table table-bordered table-striped"> <!-- table-condensed table-hover -->\n')
    f.write('        <thead>\n')
    f.write('          <tr>\n')
    f.write('            <th>#</th>\n')
    f.write('            <th>Key</th>\n')
    for atrb in atrb_list:
        if atrb.tableshow == False:
            continue
        f.write('            <th>' + atrb.name + '</th>\n')
    f.write('            <th>Action</th>\n')
    f.write('          </tr>\n')
    f.write('        </thead>\n')
    f.write('        <tbody>\n')
    f.write('          <tr ng-repeat="x in aaData">\n')
    f.write('            <td>{{ $index + 1 }}</td>\n')
    f.write('            <td>{{ x.Key }}</td>\n')
    for atrb in atrb_list:
        if atrb.tableshow == False:
            continue
        if atrb.type == 'enum':
            f.write('            <td>{{ ' + atrb.name + 'Enum[x.' + atrb.name + '] }}</td>\n')
        else:
            f.write('            <td>{{ x.' + atrb.name + ' }}</td>\n')
    f.write('            <td>\n')
    f.write('              <a ui_sref="app.' + mname + 'View({id:{{ x.id }}})" class="text-info">View & Edit</a>\n')
    f.write('            </td>\n')
    f.write('          </tr>\n')
    f.write('        </tbody>\n')
    f.write('      </table>\n')
    f.write('    </div>\n')
    f.write('  </div>\n')
    f.write('</div>\n')
    f.write('\n')
    f.close()

def gen_view_html(mname, enum_list, atrb_list):
    filename = '../html/tpl/' + mname + 'View.html'
    f = open(filename, 'w')
    f.write('<script>\n')
    f.write('\'use strict\';\n')
    f.write('app.controller(\'FormCtrl\', [\'$scope\', \'$http\', \'$stateParams\', function($scope, $http, $stateParams) {\n')
    f.write('    var loadData = function () {\n')
    f.write('        $http.get(\'/action/' + mname + '?id=\' + $stateParams.id)\n')
    f.write('        .success(function(data){$scope.formData = data.aaData[0];})\n')
    f.write('    };\n')
    f.write('    loadData();\n')
    f.write('    $scope.refresh = function() { loadData(); };\n')
    f.write('\n')
    f.write('    $scope.processForm = function() {\n')
    f.write('        $scope.message = "command inprogress";\n')
    f.write('        $http({\n')
    f.write('            method  : \'POST\',\n')
    f.write('            url     : \'/action/' + mname + '?id=\' + $stateParams.id,\n')
    f.write('            data    : $.param($scope.formData),  // pass in data as strings\n')
    f.write('            headers : { \'Content-Type\': \'application/x-www-form-urlencoded\' }  // set the headers so angular passing info as form data (not request payload)\n')
    f.write('        })\n')
    f.write('        .success(function(data) {\n')
    f.write('            //console.log(data);\n')
    f.write('            if (!data.success) {\n')
    f.write('                $scope.message = data.message;\n')
    f.write('            } else {\n')
    f.write('                $scope.message = data.message;\n')
    f.write('            }\n')
    f.write('        });\n')
    f.write('    };\n')
    f.write('}]);\n')
    f.write('</script>\n')
    f.write('\n')
    f.write('<div class="bg-light lter b-b wrapper-md">\n')
    f.write('  <h1 class="m-n font-thin h3">' + mname + ' View</h1>\n')
    f.write('</div>\n')
    f.write('<div class="wrapper-md" ng-controller="FormCtrl">\n')
    f.write('  <div class="panel panel-default">\n')
    f.write('    <div class="panel-body">\n')
    f.write('      <form class="form-horizontal" method="get" ng-submit="processForm()">\n')
    f.write('\n')
    f.write('        <div class="form-group">\n')
    f.write('          <label class="col-lg-2 control-label">Key</label>\n')
    f.write('          <div class="col-lg-10">\n')
    f.write('            <input class="form-control" type="text" ng-model="formData.Key" disabled>\n')
    f.write('          </div>\n')
    f.write('        </div>\n')
    f.write('        <div class="line line-dashed b-b line-lg pull-in"></div>\n')
    f.write('\n')
    for atrb in atrb_list:
        disabled = ''
        if atrb.readonly == True:
            disabled = ' disabled'
        if atrb.type == 'enum':
            f.write('        <div class="form-group">\n')
            f.write('          <label class="col-sm-2 control-label">' + atrb.name + '</label>\n')
            f.write('          <div class="col-sm-10">\n')
            f.write('            <select name="account" class="form-control m-b" ng-model="formData.' + atrb.name + '"' + disabled + '>\n')
            for enum in enum_list:
                if atrb.name == enum[0]:
                    for item in enum[1]:
                        f.write('              <option value="' + item.value + '">' + item.name + '</option>\n')
                    break;
            f.write('            </select>\n')
            f.write('          </div>\n')
            f.write('        </div>\n')
            f.write('        <div class="line line-dashed b-b line-lg pull-in"></div>\n')
            f.write('\n')
        else:
            f.write('        <div class="form-group">\n')
            f.write('          <label class="col-lg-2 control-label">' + atrb.name + '</label>\n')
            f.write('          <div class="col-lg-10">\n')
            f.write('            <input class="form-control" type="text" ng-model="formData.' + atrb.name + '"' + disabled + '>\n')
            f.write('          </div>\n')
            f.write('        </div>\n')
            f.write('        <div class="line line-dashed b-b line-lg pull-in"></div>\n')
            f.write('\n')
    f.write('        <div class="form-group">\n')
    f.write('          <div class="col-sm-4 col-sm-offset-2">\n')
    f.write('            <!--<button type="submit" class="btn btn-default">Cancel</button>-->\n')
    f.write('            <button type="submit" class="btn btn-primary" ng-click="submit()">Save changes</button>\n')
    f.write('            <button type="button" class="btn btn-info" ng-click="refresh()">Refresh</button>\n')
    f.write('          </div>\n')
    f.write('        </div>\n')
    f.write('        <!-- SHOW ERROR/SUCCESS MESSAGES xxDebug -->\n')
    f.write('        <div id="messages" ng-show="message">{{ message }}</div>\n')
    f.write('      </form>\n')
    f.write('    </div>\n')
    f.write('  </div>\n')
    #f.write('  <pre> {{ formData }} </pre>\n')
    f.write('</div>\n')
    f.write('\n')
    f.close()

def gen_web_handle(mob_list):
    filename = '../WEB/src/WebAction.cpp'
    f = open(filename, 'w')
    f.write('#include "Web.h"\n')
    f.write('\n')
    for mob_name in mob_list:
        f.write('extern void Web' + mob_name + 'Action(Webs *wp);\n')
    f.write('extern void WebCounterAction(Webs *wp);\n')
    f.write('extern void WebAlarmAction(Webs *wp);\n')
    f.write('\n')
    f.write('void webRegisterAction(void)\n')
    f.write('{\n')
    for mob_name in mob_list:
        f.write('    websDefineAction("' + mob_name + '", (void*)Web' + mob_name + 'Action);\n')
    f.write('    websDefineAction("Counter", (void*)WebCounterAction);\n')
    f.write('    websDefineAction("Alarm", (void*)WebAlarmAction);\n')
    f.write('}\n')
    f.close()

def gen_snmp_handle(mob_list):
    filename = '../SNMP/src/MibCollecter.cpp'
    f = open(filename, 'w')
    f.write('#include "Snmp.h"\n')
    f.write('\n')
    for mob_name in mob_list:
        f.write('extern void Init' + mob_name + 'Mib();\n')
    #f.write('extern void WebCounterAction(Webs *wp);\n')
    #f.write('extern void WebAlarmAction(Webs *wp);\n')
    f.write('\n')
    f.write('void InitMibs(void)\n')
    f.write('{\n')
    for mob_name in mob_list:
        f.write('    Init' + mob_name + 'Mib();\n')
    #f.write('    websDefineAction("Counter", (void*)WebCounterAction);\n')
    #f.write('    websDefineAction("Alarm", (void*)WebAlarmAction);\n')
    f.write('}\n')
    f.close()

def gen_mob_header(mob_list):
    filename = '../DBF/inc/MobHeader.h'
    f = open(filename, 'w')
    f.write('#ifndef _MOB_HEADER_H_\n')
    f.write('#define _MOB_HEADER_H_\n')
    f.write('\n')
    for mob_name in mob_list:
        f.write('#include "' + mob_name + 'Mob.h"\n')
    f.write('\n')
    f.write('#endif\n')
    f.close()

def gen_mib_head():
    s = open(mibHeadFile, 'r');
    d = open(mibFileName, 'w');
    try:
        c = s.read()
        d.write(c)
    finally:
        s.close()
        d.close()

def gen_mib_entry(mname, enum_list, atrb_list):
    global mibIndex
    entryIndex = 1
    f = open(mibFileName, 'a')
    f.write('--\n')
    f.write('-- ' + mname + ' mib\n')
    f.write('--\n')
    f.write(mname.lower() + ' OBJECT IDENTIFIER ::= { mxp ' + str(mibIndex) + ' }\n')
    f.write('\n')
    for atrb in atrb_list:
        f.write(mname.lower() + atrb.name + ' OBJECT-TYPE\n')
        if atrb.type == 'enum' or atrb.type == 'int':
            f.write('    SYNTAX INTEGER\n')
        else:
            f.write('    SYNTAX DisplayString\n')
        if atrb.type == 'enum':
            f.write('    {\n')
            for enum in enum_list:
                if atrb.name == enum[0]:
                    for item in enum[1]:
                        f.write('        ' + item.name.lower() + ' (' + item.value + '),\n')
            f.write('    }\n')
        if atrb.readonly == True:
            f.write('    MAX-ACCESS read-only\n')
        else:
            f.write('    MAX-ACCESS read-write\n')
        f.write('    STATUS current\n')
        f.write('    DESCRIPTION ""\n')
        f.write('    ::= { ' + mname.lower() + ' ' + str(entryIndex) + ' }\n')
        f.write('\n')
        entryIndex = entryIndex + 1
    mibIndex = mibIndex + 1
    f.close()

def gen_mib_table(mname, enum_list, atrb_list):
    global mibIndex
    entryIndex = 1
    f = open(mibFileName, 'a')
    f.write('--\n')
    f.write('-- ' + mname + ' mib\n')
    f.write('--\n')
    f.write(mname.lower() + ' OBJECT IDENTIFIER ::= { mxp ' + str(mibIndex) + ' }\n')
    f.write('\n')
    f.write(mname.lower() + 'Table OBJECT-TYPE\n')
    f.write('    SYNTAX SEQUENCE OF ' + mname + 'Entry\n')
    f.write('    MAX-ACCESS not-accessible\n')
    f.write('    STATUS current\n')
    f.write('    DESCRIPTION ""\n')
    f.write('    ::= { ' + mname.lower() + ' 1 }\n')
    f.write('\n')
    f.write(mname.lower() + 'Entry OBJECT-TYPE\n')
    f.write('    SYNTAX ' + mname + 'Entry\n')
    f.write('    MAX-ACCESS not-accessible\n')
    f.write('    STATUS current\n')
    f.write('    DESCRIPTION ""\n')
    #f.write('    INDEX { shelfId,slotId,subslotId,portId }\n')
    f.write('    INDEX { instance }\n')
    f.write('    ::= { ' + mname.lower() + 'Table 1 }\n')
    f.write('\n')
    f.write(mname + 'Entry ::=\n')
    f.write('    SEQUENCE {\n')
    f.write('        ' + mname.lower() + 'Key DisplayString,\n')
    for atrb in atrb_list:
        if atrb.type == 'enum' or atrb.type == 'int':
            f.write('        ' + mname.lower() + atrb.name + ' INTEGER,\n')
        else:
            f.write('        ' + mname.lower() + atrb.name + ' DisplayString,\n')
    f.write('    }\n')
    f.write('\n')
    f.write(mname.lower() + 'Key OBJECT-TYPE\n')
    f.write('    SYNTAX DisplayString\n')
    f.write('    MAX-ACCESS read-only\n')
    f.write('    STATUS current\n')
    f.write('    DESCRIPTION ""\n')
    f.write('    ::= { ' + mname.lower() + 'Entry ' + str(entryIndex) + ' }\n')
    f.write('\n')
    entryIndex = entryIndex + 1
    for atrb in atrb_list:
        f.write(mname.lower() + atrb.name + ' OBJECT-TYPE\n')
        if atrb.type == 'enum' or atrb.type == 'int':
            f.write('    SYNTAX INTEGER\n')
        else:
            f.write('    SYNTAX DisplayString\n')
        if atrb.type == 'enum':
            f.write('    {\n')
            for enum in enum_list:
                if atrb.name == enum[0]:
                    for item in enum[1]:
                        f.write('        ' + item.name.lower() + ' (' + item.value + '),\n')
            f.write('    }\n')
        if atrb.readonly == True:
            f.write('    MAX-ACCESS read-only\n')
        else:
            f.write('    MAX-ACCESS read-write\n')
        f.write('    STATUS current\n')
        f.write('    DESCRIPTION ""\n')
        f.write('    ::= { ' + mname.lower() + 'Entry ' + str(entryIndex) + ' }\n')
        f.write('\n')
        entryIndex = entryIndex + 1
    mibIndex = mibIndex + 1
    f.close()

def gen_mib_foot():
    s = open(mibFootFile, 'r');
    d = open(mibFileName, 'a');
    try:
        c = s.read()
        d.write(c)
    finally:
        s.close()
        d.close()

def print_node(node):
    print(node.tag, node.attrib, node.text)

def read_xml(xmlfile):
    """read xml file"""
    tree = ElementTree.parse(xmlfile)
    root = tree.getroot()

    gen_mib_head()

    mobs = []
    mob_list = root.iter("MOB")
    for mob in mob_list:
        mobs.append(mob.attrib['name'])
        mob_name = mob.attrib['name'] + 'Mob'
        mname = mob.attrib['name']
        atrb_list = []
        enum_list = []
        alarm_list = []
        counter_list = []
        for atrb in mob:
            if atrb.tag == 'alarm':
                m = AlarmObj(atrb.attrib['name'], atrb.attrib['sa'], atrb.attrib['severity'], atrb.attrib['desp'])
                alarm_list.append(m)
                continue
            if atrb.tag == 'counter':
                m = CounterObj(atrb.attrib['name'], atrb.attrib['unit'])
                counter_list.append(m)
                continue
            a = AtrbObj(atrb.attrib['name'], atrb.tag)
            if atrb.attrib.has_key('readonly') and atrb.attrib['readonly'] == 'true':
                a.readonly = True
            if atrb.attrib.has_key('persistent') and atrb.attrib['persistent'] == 'true':
                a.persistent = True
            if atrb.attrib.has_key('tableshow') and atrb.attrib['tableshow'] == 'false':
                a.tableshow = False
            atrb_list.append(a)
            if a.type == 'enum':
                item_list = []
                for enum in atrb:
                    e = EnumItemObj(enum.text, enum.attrib['value'])
                    item_list.append(e)
                enum_list.append([a.name, item_list])

        gen_mob_h(mob_name, enum_list, atrb_list, alarm_list, counter_list)
        gen_mob_cpp(mob_name, enum_list, atrb_list, alarm_list, counter_list)
        gen_page_cpp(mname, enum_list, atrb_list)
        gen_table_html(mname, enum_list, atrb_list)
        gen_view_html(mname, enum_list, atrb_list)
        if mob.attrib.has_key('single') and mob.attrib['single'] == 'true':
            gen_snmp_cpp(mname, enum_list, atrb_list)
            gen_mib_entry(mname, enum_list, atrb_list)
        else:
            gen_snmpTable_cpp(mname, enum_list, atrb_list)
            gen_mib_table(mname, enum_list, atrb_list)
    gen_web_handle(mobs)
    gen_snmp_handle(mobs)
    gen_mob_header(mobs)
    gen_mib_foot()

def CopyModifyMibFile(v):
    uName = v['name'].upper()
    uProduct = v['product'].upper()
    lName = v['name'].lower()
    lProduct = v['product'].lower()
    mibName = uName + '-' + uProduct + '-MIB'

    s = open(mibFileName, 'r');
    d = open(mibName + '.mib', 'w');
    try:
        while True:
            l = s.readline()
            if not l:
                break

            l = l.replace('19293', v['oid']);
            l = l.replace('mxp', lProduct);
            l = l.replace('danriver', lName);
            l = l.replace('DANRIVER-MXP-MIB', mibName);

            d.write(l)
    finally:
        s.close()
        d.close()

if __name__ == '__main__':
    read_xml("database.xml")
    for v in vendorList:
        CopyModifyMibFile(v)

