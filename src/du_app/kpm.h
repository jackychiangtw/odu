#ifndef _KPM_H_
#define _KPM_H_
#include "E2AP-PDU.h"
#include "E2setupRequest.h"
#include "RANfunction-Name.h"
#include "E2SM-KPM-RANfunction-Description.h"
#include "ProtocolIE-Field.h"
#include "RANfunctionOID.h"
#include "GlobalE2node-gNB-ID.h"
#include "ProtocolIE-FieldE2.h"
#include "InitiatingMessageE2.h"
#include "SuccessfulOutcomeE2.h"
#include "odu_common_codec.h"
#include "E2nodeComponentInterfaceF1.h"
#include "RIC-EventTriggerStyle-Item.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>
#include "RANfunction-Item.h"

typedef void (*SubscriptionCallback)(E2AP_PDU_t*);

extern asn_TYPE_descriptor_t asn_DEF_E2SM_KPM_RANfunction_Description;

//void callback_kpm_subscription_request(E2AP_PDU_t *pdu);
//void register_e2sm(long func_id, OCTET_STRING_t* ostr);
uint8_t kpm(RANfunctions_List_t  *ranfun_list);

struct ran_func_info {
    long ranFunctionId;
    OCTET_STRING_t *ranFunctionDesc;
    long ranFunctionRev;
    PrintableString_t *ranFunctionOId;
  };

//uint8_t loop_for_ranfunction(E2setupRequest_t **e2apMsg);

#endif
