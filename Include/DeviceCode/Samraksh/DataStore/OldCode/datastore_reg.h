#ifndef _DATASTORE_REG_H_INCLUDED_
#define _DATASTORE_REG_H_INCLUDED_

typedef struct _datastore_reg_entry
{
    LPVOID dataStoreHandle;    /* Chethan : Using LPVOID as class Data_Store* creates a cyclic dependance - Do something later */
    LPVOID addrRangeStart;
    LPVOID addrRangeEnd;
    /* Chethan :: Do I need anything else here? */
}DATASTORE_REG_ENTRY;

DATASTORE_STATUS DATASTORE_REG_CreateEntry(DATASTORE_REG_ENTRY *entry); /* Chethan :: Should I give somekind of handle? */
LPVOID DATASTORE_REG_LookupDataStoreHandle(LPVOID addr);                /* Look up the datastore handle based on the address */
DATASTORE_STATUS DATASTORE_REG_RemoveEntry(LPVOID addr);    

#endif