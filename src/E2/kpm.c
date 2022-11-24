#include <stdint.h>
#include <unistd.h>
//#include <nlohmann/json.hpp>
#include "kpm.h"
#include "E2SM-KPM-RANfunction-Description.h"
#include "RIC-EventTriggerStyle-Item.h"
#include "RIC-ReportStyle-Item.h"
#include "MeasurementInfo-Action-Item.h"
#include "ProtocolIE-Field.h"
#include "RANfunction-Item.h"

//  #include "OCUCP-PF-Container.h"
#include "constr_TYPE.h"
#include "OCTET_STRING.h"
#include "asn_application.h"
#include "E2SM-KPM-IndicationMessage.h"
#include "E2SM-KPM-RANfunction-Description.h"
#include "E2SM-KPM-IndicationHeader-Format1.h"
#include "E2SM-KPM-IndicationHeader.h"
#include "E2AP-PDU.h"
#include "RICsubscriptionRequest.h"
#include "RICsubscriptionResponse.h"
#include "RICactionType.h"
#include "ProtocolIE-SingleContainer.h"
#include "InitiatingMessage.h"
#include "E2setupRequest.h"
#include "RANfunctionOID.h"
#include "GlobalE2node-gNB-ID.h"
#include "ProtocolIE-FieldE2.h"
#include "InitiatingMessageE2.h"
#include "SuccessfulOutcomeE2.h"
#include "odu_common_codec.h"
#include "E2nodeComponentInterfaceF1.h"



uint8_t kpm(RANfunctions_List_t  *ranfun_list)
{
    
    int8_t ret = 0;
    asn_codec_ctx_t *opt_cod;

    E2SM_KPM_RANfunction_Description_t *ranfunc_desc = (E2SM_KPM_RANfunction_Description_t*)calloc(1, sizeof(E2SM_KPM_RANfunction_Description_t));
    uint8_t *ranfunc_defin = (uint8_t*)"ORAN-E2SM-KPM"; // RAN Function Definition,
    uint8_t *ranfunc_descr = (uint8_t*)"KPM monitor"; // RAN function description
    uint8_t *ranfunc_oid = (uint8_t*)"OID123"; // RAN function service model OID
    long *inst;

    printf("\nINFO   -->  E2 Agent : List all RAN functions<<<<\n");
    //sleep(1000);


    ASN_STRUCT_RESET(asn_DEF_E2SM_KPM_RANfunction_Description, ranfunc_desc);

    // RAN Function Name Start

    ranfunc_desc->ranFunction_Name.ranFunction_ShortName.size = strlen((uint8_t*)ranfunc_defin);
    ranfunc_desc->ranFunction_Name.ranFunction_ShortName.buf = (uint8_t*)calloc(strlen((char*)ranfunc_defin), sizeof(uint8_t));
    memcpy(ranfunc_desc->ranFunction_Name.ranFunction_ShortName.buf, ranfunc_defin, ranfunc_desc->ranFunction_Name.ranFunction_ShortName.size);

    ranfunc_desc->ranFunction_Name.ranFunction_Description.size = strlen((uint8_t*)ranfunc_descr);
    ranfunc_desc->ranFunction_Name.ranFunction_Description.buf = (uint8_t*)calloc(strlen((char*)ranfunc_descr), sizeof(uint8_t));
    memcpy(ranfunc_desc->ranFunction_Name.ranFunction_Description.buf, ranfunc_descr, ranfunc_desc->ranFunction_Name.ranFunction_ShortName.size);



    ranfunc_desc->ranFunction_Name.ranFunction_Instance = (long*)calloc(1,sizeof(long));
    *ranfunc_desc->ranFunction_Name.ranFunction_Instance = 1;

    ranfunc_desc->ranFunction_Name.ranFunction_Description.size = strlen((uint8_t*)ranfunc_oid);
    ranfunc_desc->ranFunction_Name.ranFunction_Description.buf = (uint8_t*)calloc(strlen((char*)ranfunc_oid), sizeof(uint8_t));
    memcpy(ranfunc_desc->ranFunction_Name.ranFunction_Description.buf, ranfunc_oid, ranfunc_desc->ranFunction_Name.ranFunction_ShortName.size);

    //RAN Function Name End


    //sleep(1000);

    //Sequence of Event Trigger styles Start

    ranfunc_desc->ric_EventTriggerStyle_List = (struct E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List*)calloc(1, sizeof(struct E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List));

    RIC_EventTriggerStyle_Item_t *trigger_style;
    trigger_style = (RIC_EventTriggerStyle_Item_t*)calloc(1, sizeof(RIC_EventTriggerStyle_Item_t));

    trigger_style->ric_EventTriggerStyle_Type = 1; // KPM only supports type 1

    uint8_t *trig_name = (uint8_t*)"Periodic report";
    trigger_style->ric_EventTriggerStyle_Name.size = strlen((uint8_t*)trig_name);
    trigger_style->ric_EventTriggerStyle_Name.buf = (uint8_t*)calloc(strlen((char*)trig_name), sizeof(uint8_t));
    memcpy(trigger_style->ric_EventTriggerStyle_Name.buf, trig_name, trigger_style->ric_EventTriggerStyle_Name.size);

    trigger_style->ric_EventTriggerFormat_Type = 5;
    ASN_SEQUENCE_ADD(&ranfunc_desc->ric_EventTriggerStyle_List->list, trigger_style);


    //DU_ALLOC(ranfunc_desc->ric_EventTriggerStyle_List, sizeof(E2SM_KPM_RANfunction_Description.E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List));

    //ret = ASN_SEQUENCE_ADD(&ranfunc_desc->ric_EventTriggerStyle_List->list , trigger_style);
    //printf("ret is %d\n", ret);

    // Sequence of Event Trigger styles End

    //sleep(1000);

    // Sequence of Report style Start

    ranfunc_desc->ric_ReportStyle_List = (struct E2SM_KPM_RANfunction_Description__ric_ReportStyle_List*)calloc(1, sizeof(struct E2SM_KPM_RANfunction_Description__ric_ReportStyle_List));
    RIC_ReportStyle_Item_t *report_style = (RIC_ReportStyle_Item_t*)calloc(1, sizeof(RIC_ReportStyle_Item_t));

    report_style->ric_ReportStyle_Type = 1;

    uint8_t *report_name = (uint8_t*)"O-DU Measurement Container for the 5GC connected deployment";

    report_style->ric_ReportStyle_Name.size = strlen((uint8_t*)report_name);
    report_style->ric_ReportStyle_Name.buf = (uint8_t*)calloc(strlen((char*)report_name), sizeof(uint8_t));
    memcpy(report_style->ric_ReportStyle_Name.buf, report_name, report_style->ric_ReportStyle_Name.size);
    report_style->ric_IndicationHeaderFormat_Type = 1;
    report_style->ric_IndicationMessageFormat_Type = 1;

    report_style->ric_ActionFormat_Type = 1; // new in KPM 2.0

    MeasurementInfo_Action_Item_t *measure_item;
    measure_item = (MeasurementInfo_Action_Item_t*)calloc(1, sizeof(MeasurementInfo_Action_Item_t));

    uint8_t *measure_name = "TotalofAvaliablePRBs";
    measure_item->measName.size = strlen((uint8_t*)measure_name);
    measure_item->measName.buf = (uint8_t*)calloc(strlen((char*)measure_name), sizeof(uint8_t));
    memcpy(measure_item->measName.buf, measure_name, measure_item->measName.size);


    ASN_SEQUENCE_ADD(&report_style->measInfo_Action_List.list, measure_item);

    ASN_SEQUENCE_ADD(&ranfunc_desc->ric_ReportStyle_List->list, report_style);

    // Sequence of Report style End


    //sleep(1000);

    xer_fprint(stderr, &asn_DEF_E2SM_KPM_RANfunction_Description, ranfunc_desc);

    
    RANfunction_ItemIEs_t *ranfunc_item;
    ranfunc_item = (RANfunction_ItemIEs_t*)calloc(1, sizeof(RANfunction_ItemIEs_t));
    
    //ASN_STRUCT_RESET(asn_DEF_RANfunction_ItemIEs, ranfunc_kpm);
    ranfunc_item->id = ProtocolIE_IDE2_id_RANfunction_Item;
    ranfunc_item->criticality = Criticality_reject;
    ranfunc_item->value.present = RANfunction_ItemIEs__value_PR_RANfunction_Item;
    ranfunc_item->value.choice.RANfunction_Item.ranFunctionID = 1;

    ranfunc_item->value.choice.RANfunction_Item.ranFunctionOID.size = strlen((uint8_t*)ranfunc_oid);
    ranfunc_item->value.choice.RANfunction_Item.ranFunctionOID.buf = (uint8_t*)calloc(strlen((char*)ranfunc_oid), sizeof(uint8_t));
    memcpy(ranfunc_item->value.choice.RANfunction_Item.ranFunctionOID.buf, ranfunc_oid, ranfunc_item->value.choice.RANfunction_Item.ranFunctionOID.size);

    uint8_t e2smbuffer[8192] = {0, };
    size_t e2smbuffer_size = 8192;
    asn_enc_rval_t er =
    asn_encode_to_buffer(opt_cod,
         ATS_ALIGNED_BASIC_PER,
         &asn_DEF_E2SM_KPM_RANfunction_Description,
         ranfunc_desc, e2smbuffer, e2smbuffer_size);

    uint8_t *ranfuncdesc = (uint8_t*)calloc(1,er.encoded);
    memcpy(ranfuncdesc, e2smbuffer, er.encoded);

    OCTET_STRING_t *ranfunc_ostr = (OCTET_STRING_t*)calloc(1,sizeof(OCTET_STRING_t));
    ranfunc_ostr->buf = (uint8_t*)calloc(1,er.encoded);
    ranfunc_ostr->size = er.encoded;
    memcpy(ranfunc_ostr->buf,e2smbuffer,er.encoded);

    ranfunc_item->value.choice.RANfunction_Item.ranFunctionDefinition.size = (uint8_t*)ranfunc_ostr->size;
    ranfunc_item->value.choice.RANfunction_Item.ranFunctionDefinition.buf = (uint8_t*)calloc(ranfunc_ostr->size, sizeof(uint8_t));
    memcpy(ranfunc_item->value.choice.RANfunction_Item.ranFunctionDefinition.buf, ranfunc_ostr->buf, ranfunc_item->value.choice.RANfunction_Item.ranFunctionDefinition.size);

    ranfunc_item->value.choice.RANfunction_Item.ranFunctionRevision = 1;  
    
    xer_fprint(stderr, &asn_DEF_RANfunction_ItemIEs, ranfunc_item);

    ASN_SEQUENCE_ADD(&ranfun_list->list, ranfunc_item);
    
    //sleep(1000);

    return ret;
}
