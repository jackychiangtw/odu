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
#include "E2SM-KPM-ActionDefinition-Format1.h"

//  #include "OCUCP-PF-Container.h"
#include "constr_TYPE.h"
#include "OCTET_STRING.h"
#include "asn_application.h"
#include "E2SM-KPM-IndicationMessage.h"
#include "E2SM-KPM-RANfunction-Description.h"
#include "E2SM-KPM-IndicationHeader-Format1.h"
#include "E2SM-KPM-EventTriggerDefinition.h"
#include "E2SM-KPM-EventTriggerDefinition-Format1.h"
#include "E2SM-KPM-IndicationHeader.h"
#include "E2SM-KPM-ActionDefinition.h"
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

uint8_t fillRanFunctionName(RANfunction_Name_t *ranfunc_name){
    uint8_t ranfunc_defin[] = "ORAN-E2SM-KPM"; // RAN Function Definition,
    uint8_t ranfunc_descr[] = "KPM monitor"; // RAN function description
    uint8_t ranfunc_oid[] = "2.0.0"; // RAN function service model OID

    ranfunc_name->ranFunction_ShortName.size = sizeof(ranfunc_defin);
    ranfunc_name->ranFunction_ShortName.buf = (uint8_t*)calloc(sizeof(ranfunc_defin), sizeof(uint8_t));
    memcpy(ranfunc_name->ranFunction_ShortName.buf, ranfunc_defin, ranfunc_name->ranFunction_ShortName.size);

    ranfunc_name->ranFunction_Description.size = sizeof(ranfunc_descr);
    ranfunc_name->ranFunction_Description.buf = (uint8_t*)calloc(sizeof(ranfunc_descr), sizeof(uint8_t));
    memcpy(ranfunc_name->ranFunction_Description.buf, ranfunc_descr, ranfunc_name->ranFunction_Description.size);

    ranfunc_name->ranFunction_Instance = (long*)calloc(1,sizeof(long));
    *ranfunc_name->ranFunction_Instance = 1;

    ranfunc_name->ranFunction_E2SM_OID.size = sizeof(ranfunc_oid);
    ranfunc_name->ranFunction_E2SM_OID.buf = (uint8_t*)calloc(sizeof(ranfunc_oid), sizeof(uint8_t));
    memcpy(ranfunc_name->ranFunction_E2SM_OID.buf, ranfunc_oid, ranfunc_name->ranFunction_E2SM_OID.size);

}

uint8_t fillTriggerStyle(RIC_EventTriggerStyle_Item_t **trigger_style){
    *trigger_style = (RIC_EventTriggerStyle_Item_t*)calloc(1, sizeof(RIC_EventTriggerStyle_Item_t));
    (*trigger_style)->ric_EventTriggerStyle_Type = 1; // KPM only supports type 1

    uint8_t trig_name[] = "Periodic report";
    (*trigger_style)->ric_EventTriggerStyle_Name.size = sizeof(trig_name);
    (*trigger_style)->ric_EventTriggerStyle_Name.buf = (uint8_t*)calloc(sizeof(trig_name), sizeof(uint8_t));
    memcpy((*trigger_style)->ric_EventTriggerStyle_Name.buf, trig_name, (*trigger_style)->ric_EventTriggerStyle_Name.size);

    (*trigger_style)->ric_EventTriggerFormat_Type = 5;

}



uint8_t kpm(RANfunctions_List_t  *ranfun_list)
{
    uint8_t arrIdx = 0;
    int8_t ret = 0;
    asn_codec_ctx_t *opt_cod;

    E2SM_KPM_RANfunction_Description_t *ranfunc_desc = (E2SM_KPM_RANfunction_Description_t*)malloc(sizeof(E2SM_KPM_RANfunction_Description_t));
    uint8_t ranfunc_defin[] = "ORAN-E2SM-KPM"; // RAN Function Definition,
    uint8_t ranfunc_descr[] = "KPM monitor"; // RAN function description
    uint8_t ranfunc_oid[] = "2.0.0"; // RAN function service model OID
    long *inst;

    printf("\nINFO   -->  E2 Agent : List all RAN functions<<<<\n");
    //sleep(1000);
    ASN_STRUCT_RESET(asn_DEF_E2SM_KPM_RANfunction_Description, ranfunc_desc);

    // RAN Function Name Start
    fillRanFunctionName(&ranfunc_desc->ranFunction_Name);

    //RAN Function Name End
    //Sequence of Event Trigger styles Start
    RIC_EventTriggerStyle_Item_t *trigger_style;
    fillTriggerStyle(&trigger_style);
        
    ranfunc_desc->ric_EventTriggerStyle_List = (struct E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List*)calloc(1, sizeof(struct E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List));
    ASN_SEQUENCE_ADD(&ranfunc_desc->ric_EventTriggerStyle_List->list, trigger_style);
    // Sequence of Event Trigger styles End


    // Sequence of Report style Start

    ranfunc_desc->ric_ReportStyle_List = (struct E2SM_KPM_RANfunction_Description__ric_ReportStyle_List*)calloc(1, sizeof(struct E2SM_KPM_RANfunction_Description__ric_ReportStyle_List));
    RIC_ReportStyle_Item_t *report_style = (RIC_ReportStyle_Item_t*)calloc(1, sizeof(RIC_ReportStyle_Item_t));

    report_style->ric_ReportStyle_Type = 1;
    uint8_t report_name[] = "O-DU Measurement Container for the 5GC connected deployment";

    report_style->ric_ReportStyle_Name.size = sizeof(report_name);
    report_style->ric_ReportStyle_Name.buf = (uint8_t*)calloc(sizeof(report_name), sizeof(uint8_t));
    memcpy(report_style->ric_ReportStyle_Name.buf, report_name, report_style->ric_ReportStyle_Name.size);
    report_style->ric_IndicationHeaderFormat_Type = 1;
    report_style->ric_IndicationMessageFormat_Type = 1;
    report_style->ric_ActionFormat_Type = 1; // new in KPM 2.0

    MeasurementInfo_Action_Item_t *measure_item;
    measure_item = (MeasurementInfo_Action_Item_t*)calloc(1, sizeof(MeasurementInfo_Action_Item_t));

    uint8_t measure_name[] = "TotalofAvaliablePRBs";
    measure_item->measName.size = sizeof(measure_name);
    measure_item->measName.buf = (uint8_t*)calloc(sizeof(measure_name), sizeof(uint8_t));
    memcpy(measure_item->measName.buf, measure_name, measure_item->measName.size);

    ASN_SEQUENCE_ADD(&report_style->measInfo_Action_List.list, measure_item);
    ASN_SEQUENCE_ADD(&ranfunc_desc->ric_ReportStyle_List->list, report_style);

    // Sequence of Report style End

    xer_fprint(stderr, &asn_DEF_E2SM_KPM_RANfunction_Description, ranfunc_desc);

    // RAN Function item in E2AP
    RANfunction_ItemIEs_t *ranfunc_item;
    ranfunc_item = (RANfunction_ItemIEs_t*)calloc(1, sizeof(RANfunction_ItemIEs_t));
    ranfunc_item->id = ProtocolIE_IDE2_id_RANfunction_Item;
    ranfunc_item->criticality = Criticality_reject;
    ranfunc_item->value.present = RANfunction_ItemIEs__value_PR_RANfunction_Item;
    ranfunc_item->value.choice.RANfunction_Item.ranFunctionID = 1;

    ranfunc_item->value.choice.RANfunction_Item.ranFunctionOID.size = sizeof(ranfunc_oid);
    ranfunc_item->value.choice.RANfunction_Item.ranFunctionOID.buf = (uint8_t*)calloc(sizeof(ranfunc_oid), sizeof(uint8_t));
    memcpy(ranfunc_item->value.choice.RANfunction_Item.ranFunctionOID.buf, ranfunc_oid, ranfunc_item->value.choice.RANfunction_Item.ranFunctionOID.size);

    // encode the E2SM-KPM message into the E2AP buffer (OCTET_STRING_t in E2AP)

    uint8_t e2smbuffer[8192];
    size_t e2smbuffer_size = 8192;
    asn_enc_rval_t     encRetVal;
    asn_dec_rval_t     decRetVal; 

    encBufSize = 0;
    //encRetVal = aper_encode_to_buffer(&asn_DEF_E2SM_KPM_RANfunction_Description, NULL, ranfunc_desc, e2smbuffer, e2smbuffer_size);

    encRetVal = asn_encode_to_buffer(opt_cod,
         ATS_ALIGNED_BASIC_PER,
         &asn_DEF_E2SM_KPM_RANfunction_Description,
         ranfunc_desc, e2smbuffer, e2smbuffer_size);
    
    //encRetVal = aper_encode(&asn_DEF_E2SM_KPM_RANfunction_Description, 0, ranfunc_desc, PrepFinalEncBuf, e2smbuffer);

    printf("\nINFO   -->  E2 Agent : Print out encode size: %d\n", encRetVal.encoded);

    OCTET_STRING_t *ranfunc_ostr = (OCTET_STRING_t*)calloc(1,sizeof(OCTET_STRING_t));
    ranfunc_ostr->buf = (uint8_t*)calloc(encRetVal.encoded, sizeof(uint8_t));
    ranfunc_ostr->size = encRetVal.encoded;
    memcpy(ranfunc_ostr->buf, e2smbuffer, encRetVal.encoded);

    ranfunc_item->value.choice.RANfunction_Item.ranFunctionDefinition.size = (uint8_t*)ranfunc_ostr->size;
    ranfunc_item->value.choice.RANfunction_Item.ranFunctionDefinition.buf = (uint8_t*)calloc(ranfunc_ostr->size, sizeof(uint8_t));
    memcpy(ranfunc_item->value.choice.RANfunction_Item.ranFunctionDefinition.buf, ranfunc_ostr->buf, ranfunc_item->value.choice.RANfunction_Item.ranFunctionDefinition.size);
    ranfunc_item->value.choice.RANfunction_Item.ranFunctionRevision = 1;  

    xer_fprint(stderr, &asn_DEF_RANfunction_ItemIEs, ranfunc_item);

    // Fill in list of RAN funcion
    ASN_SEQUENCE_ADD(&ranfun_list->list, ranfunc_item);
    
    return ret;
}

uint8_t fillMeasInfoItem(MeasurementInfoItem_t **measInfoItem){
    *measInfoItem = (MeasurementInfoItem_t*)malloc(sizeof(MeasurementInfoItem_t));
    LabelInfoItem_t *measLabelInfo = (LabelInfoItem_t*)calloc(1, sizeof(LabelInfoItem_t)); // can't use malloc
    long *measLabelInfo_sum = (long*)malloc(sizeof(long));

    (*measInfoItem)->measType.present = MeasurementType_PR_measID; // Choose name or ID
    (*measInfoItem)->measType.choice.measID = 101; // defined by user
    *measLabelInfo_sum = MeasurementLabel__sUM_true;
    measLabelInfo->measLabel.sUM = measLabelInfo_sum;

    ASN_SEQUENCE_ADD(&(*measInfoItem)->labelInfoList.list, measLabelInfo);

}


uint8_t fillActionDefinition(RICactionDefinition_t **ricdifin){ // 
    asn_enc_rval_t     encRetVal;
    asn_codec_ctx_t *opt_cod;
    E2SM_KPM_ActionDefinition_Format1_t *actionDefinFormat = (E2SM_KPM_ActionDefinition_Format1_t*)calloc(1, sizeof(E2SM_KPM_ActionDefinition_Format1_t));
    *ricdifin = (RICactionDefinition_t*)malloc(sizeof(RICactionDefinition_t));

    printf("\nINFO   -->  E2 Agent : Print Action Definition<<<<\n");
    actionDefinFormat->granulPeriod = 100; // 100ms

    MeasurementInfoItem_t *measInfoItem;
    fillMeasInfoItem(&measInfoItem);

    ASN_SEQUENCE_ADD(&actionDefinFormat->measInfoList.list, measInfoItem);

    uint8_t e2smbuffer[8192] = {0, };
    size_t e2smbuffer_size = 8192;

    // Encode to OCTET STRING

    asn_enc_rval_t er =
    asn_encode_to_buffer(opt_cod,
         ATS_ALIGNED_BASIC_PER,
         &asn_DEF_E2SM_KPM_ActionDefinition_Format1,
         actionDefinFormat, e2smbuffer, e2smbuffer_size);

    OCTET_STRING_t *ricdifin_ostr = (OCTET_STRING_t*)calloc(1,sizeof(OCTET_STRING_t));
    ricdifin_ostr->buf = (uint8_t*)calloc(1,er.encoded);
    ricdifin_ostr->size = er.encoded;
    memcpy(ricdifin_ostr->buf,e2smbuffer,er.encoded);

    (*ricdifin)->size = ricdifin_ostr->size;
    (*ricdifin)->buf = (uint8_t*)malloc(ricdifin_ostr->size);
    memcpy((*ricdifin)->buf, ricdifin_ostr->buf, (*ricdifin)->size);

    xer_fprint(stderr, &asn_DEF_E2SM_KPM_ActionDefinition_Format1, actionDefinFormat);


    printf("\nINFO   -->  E2 Agent : Print Action Definition End<<<<\n");

}

uint8_t fillEventTrigDefinitionFormat1(RICeventTriggerDefinition_t **eventTrigDefi){

    E2SM_KPM_EventTriggerDefinition_t *kpmEventTrig = (E2SM_KPM_EventTriggerDefinition_t*)calloc(1, sizeof(E2SM_KPM_EventTriggerDefinition_t));
    E2SM_KPM_EventTriggerDefinition_Format1_t *kpmEventTrigFormat1 = (E2SM_KPM_EventTriggerDefinition_Format1_t*)calloc(1, sizeof(E2SM_KPM_EventTriggerDefinition_Format1_t));

    printf("\nINFO   -->  E2 Agent : Fill Event Trigger Definition<<<<\n");

    kpmEventTrig->eventDefinition_formats.present = E2SM_KPM_EventTriggerDefinition__eventDefinition_formats_PR_eventDefinition_Format1;
    kpmEventTrig->eventDefinition_formats.choice.eventDefinition_Format1 = kpmEventTrigFormat1;
    kpmEventTrigFormat1->reportingPeriod = 1000; // 1000ms

    uint8_t e2smbuffer[8192] = {0, };
    size_t e2smbuffer_size = 8192;
    asn_codec_ctx_t *opt_cod;

    asn_enc_rval_t er =
    asn_encode_to_buffer(opt_cod,
         ATS_ALIGNED_BASIC_PER,
         &asn_DEF_E2SM_KPM_EventTriggerDefinition,
         kpmEventTrig, e2smbuffer, e2smbuffer_size);

    //*eventTrigDefi = (RICeventTriggerDefinition_t*)calloc(1, sizeof(RICeventTriggerDefinition_t));

    (*eventTrigDefi)->buf = (uint8_t*)calloc(1,er.encoded);
    (*eventTrigDefi)->size = er.encoded;
    memcpy((*eventTrigDefi)->buf,e2smbuffer,er.encoded);

    xer_fprint(stderr, &asn_DEF_E2SM_KPM_EventTriggerDefinition, kpmEventTrig);
    printf("\nINFO   -->  E2 Agent : Event Trigger Definition End<<<<\n");

}


uint8_t decapEventTrigDefinitionFormat1(RICeventTriggerDefinition_t *defini){
    asn_dec_rval_t  rval;
    E2SM_KPM_EventTriggerDefinition_t *kpmEventTrig = (E2SM_KPM_EventTriggerDefinition_t*)calloc(1, sizeof(E2SM_KPM_EventTriggerDefinition_t));

    printf("\nINFO   -->  E2 Agent : Decap Event Trigger Definition<<<<\n");

    rval = aper_decode(0, &asn_DEF_E2SM_KPM_EventTriggerDefinition, (void **)&kpmEventTrig, defini->buf, defini->size, 0, 0);
    if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      printf("\nERROR  -->  E2 Agent : ASN decode failed");
   }
    
    xer_fprint(stdout, &asn_DEF_E2SM_KPM_EventTriggerDefinition, kpmEventTrig);
    printf("\nINFO  -->  E2 Agent : Report Period is %d\n", kpmEventTrig->eventDefinition_formats.choice.eventDefinition_Format1->reportingPeriod);


}

uint8_t decapActionDefinition(RICactionDefinition_t *ricdifin){
    asn_dec_rval_t  rval;
    E2SM_KPM_ActionDefinition_Format1_t *actionDefinFormat = (E2SM_KPM_ActionDefinition_Format1_t*)calloc(1, sizeof(E2SM_KPM_ActionDefinition_Format1_t));

    printf("\nINFO   -->  E2 Agent : Decap Action Definition<<<<\n");

    //xer_fprint(stdout, &asn_DEF_RICactionDefinition, ricdifin);

    rval = aper_decode(0, &asn_DEF_E2SM_KPM_ActionDefinition_Format1, (void **)&actionDefinFormat, ricdifin->buf, ricdifin->size, 0, 0);
    if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      printf("\nERROR  -->  E2 Agent : ASN decode failed\n");
   }
    
    xer_fprint(stdout, &asn_DEF_E2SM_KPM_ActionDefinition_Format1, actionDefinFormat);

    printf("\nINFO  -->  E2 Agent : Measurement ID is %d", actionDefinFormat->measInfoList.list.array[0]->measType.choice.measID);
    //printf("\nERROR  -->  E2 Agent : Measurement Label Sum is %d\n", *(actionDefinFormat->measInfoList.list.array[0]->labelInfoList.list.array[0]->measLabel.sUM));
    printf("\nINFO  -->  E2 Agent : Granularity Period is %d\n", actionDefinFormat->granulPeriod);

}