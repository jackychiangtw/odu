/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/* This file contains E2AP message handler functions */

#include "ric_stub_sctp.h"
#include "ric_e2ap_msg_hdl.h"
#include "GlobalE2node-gNB-ID.h"

char encBuf[ENC_BUF_MAX_LEN];
Bool ricSubsStatus;
S16 SendE2APMsg(Region , Pool );

/*******************************************************************
 *
 * @brief Writes the encoded chunks into a buffer
 *
 * @details
 *
 *    Function : PrepFinalEncBuf
 *
 *    Functionality:Fills the encoded buffer
 *
 * @params[in] void *buffer,initial encoded data
 * @params[in] size_t size,size of buffer
 * @params[in] void *encodedBuf,final buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
static int PrepFinalEncBuf(const void *buffer, size_t size, void *encodedBuf)
{
   memcpy(encodedBuf + encBufSize, buffer, size);
   encBufSize += size;
   return 0;
} /* PrepFinalEncBuf */

/*******************************************************************
*
* @brief Sends E2 msg over SCTP
*
* @details
*
*    Function : SendE2APMsg
*
*    Functionality: Sends E2 msg over SCTP
*
* @params[in] Region region
*             Pool pool
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

S16 SendE2APMsg(Region region, Pool pool)
{
   Buffer *mBuf;

   if(SGetMsg(region, pool, &mBuf) == ROK)
   {
      if(SAddPstMsgMult((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         SPrntMsg(mBuf, 0,0);
 
         if(sctpSend(mBuf) != ROK)
         {
            DU_LOG("\nF1AP : SCTP Send for E2  failed");
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
      }
      else
      {
         DU_LOG("\nF1AP : SAddPstMsgMult failed");
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      SPutMsg(mBuf);
   }
   else
   {
      DU_LOG("\nF1AP : Failed to allocate memory");
      RETVALUE(RFAILED);
   }
 
   RETVALUE(ROK);
} /* SendE2APMsg */

/*******************************************************************
 *
 * @brief Builds PLMN ID 
 *
 * @details
 *
 *    Function : plmnBuildRic
 *
 *    Functionality: Building the PLMN ID
 *
 * @params[in] PLMNID plmn
 *             OCTET_STRING_t *octe
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void plmnBuildRic(Plmn plmn, OCTET_STRING_t *octe)
{
   U8 mncCnt;
   octe->size = 3;
   RIC_ALLOC(octe->buf,  octe->size * sizeof(U8));
   if(octe->buf == NULLP)
   {
      RETVALUE(RFAILED);
   }
   mncCnt = 2;
   octe->buf[0] = ((plmn.mcc[1] << 4) | (plmn.mcc[0]));
   if(mncCnt == 2)
   {
      octe->buf[1]  = ((0xf0) | (plmn.mcc[2]));
      octe->buf[2] = ((plmn.mnc[1] << 4) | (plmn.mnc[0]));
   }
   else
   {
      octe->buf[1] = ((plmn.mnc[0] << 4) | (plmn.mcc[2]));
      octe->buf[2] = ((plmn.mnc[2] << 4) | (plmn.mnc[1]));
   }
}

/*******************************************************************
 *
 * @brief Fills the RicId
 *
 * @details
 *
 *    Function : FillRicId
 *
 *    Functionality: Fills the RicId
 *
 * @params[in] BIT_STRING_t *nbid,
 *             U8 unusedBits
 *             U8 byteSize
 *             U8 val
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 FillRicId(BIT_STRING_t *id, U8 unusedBits, U8 byteSize, U8 val)
{
   U8 tmp;
   id->size = byteSize;
   RIC_ALLOC(id->buf, id->size * sizeof(U8));
   if(id->buf == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for (tmp = 0 ; tmp < (byteSize-1); tmp++)
   {
      id->buf[tmp] = tmp;
   }
   id->buf[byteSize-1]   = val;  //change this 
   id->bits_unused = unusedBits;
   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Builds Global RIC Id Params
 *
 * @details
 *
 *    Function : BuildGlobalRicId
 *
 *    Functionality: Building the Plmn and ric id
 *
 * @params[in] GlobalRIC_ID_t *ricId
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildGlobalRicId(GlobalRIC_ID_t *ricId)
{
   U8 unused = 4;
   U8 byteSize = 3;
   U8 val = 1;
   if(ricId != NULLP)
   {
      plmnBuildRic(cuCfgParams.plmn , &ricId->pLMN_Identity);
      /* fill ric Id */
      FillRicId(&ricId->ric_ID, unused, byteSize, val);
   }
   RETVALUE(ROK);   
}
#if 0
RANfunctionID_ItemIEs_t* FillRanFuncItems(RANfunctionID_ItemIEs_t *items)
{
  if(items != NULLP)
  {
     items->id = ProtocolIE_IDE2_id_RANfunction_Item;
     items->criticality = CriticalityE2_reject;
     items->value.present = RANfunctionID_ItemIEs__value_PR_RANfunctionID_Item; 
     items->value.choice.RANfunctionID_Item.ranFunctionID = 1;
     items->value.choice.RANfunctionID_Item.ranFunctionRevision = 4;
  }
  RETVALUE(items);
}

S16 BuildRANfuncIdList(RANfunctionsID_List_t *funcIdList)
{
   U8 elementCnt;
   RANfunctionID_ItemIEs_t *funcIdItems;

   elementCnt = 1; 
   funcIdList->list.count = elementCnt;
   funcIdList->list.size  = elementCnt * sizeof(RANfunctionID_ItemIEs_t);
   RIC_ALLOC(funcIdList->list.array, funcIdList->list.size);
   if(funcIdList->list.array == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for RAN Function List failed");
      RETVALUE(RFAILED);
   }
   RIC_ALLOC(funcIdList->list.array[0], sizeof(RANfunctionID_ItemIEs_t));
   FillRanFuncItems(funcIdList->list.array[0]);
   RETVALUE(ROK);
}
#endif

/*******************************************************************
 *
 * @brief Builds and sends the E2SetupResponse
 *
 * @details
 *
 *    Function : BuildAndSendE2SetupRsp
 *
 *    Functionality: Constructs the F1SetupResponse message and sends
 *                   it back to the DU through SCTP.
 *
 * @params[in] void **buf,Buffer to which encoded pattern is written into
 * @params[in] int *size,size of buffer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
S16 BuildAndSendE2SetupRsp()
{
   E2AP_PDU_t         *e2apMsg = NULL;
   E2setupResponse_t  *e2SetupRsp;
   asn_enc_rval_t     encRetVal; 
   U8 idx;
   U8 elementCnt;

 
   DU_LOG("\nE2AP : Building E2 Setup Response\n");

   RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t)); 
   if(e2apMsg == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2AP-PDU failed");
      RETVALUE(RFAILED);
   }
   e2apMsg->present =  E2AP_PDU_PR_successfulOutcome;
   RIC_ALLOC(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
   if(e2apMsg->choice.successfulOutcome == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2AP-PDU failed");
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
      RETVALUE(RFAILED);  
   }

   e2apMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_E2setup;
   e2apMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
   e2apMsg->choice.successfulOutcome->value.present = \
         SuccessfulOutcomeE2__value_PR_E2setupResponse;
   e2SetupRsp = &e2apMsg->choice.successfulOutcome->value.choice.E2setupResponse;

   elementCnt = 1;
   e2SetupRsp->protocolIEs.list.count = elementCnt;
   e2SetupRsp->protocolIEs.list.size  = elementCnt * sizeof(E2setupResponseIEs_t);

   RIC_ALLOC(e2SetupRsp->protocolIEs.list.array, \
              e2SetupRsp->protocolIEs.list.size);
   if(e2SetupRsp->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2ResponseIEs failed");
      RIC_FREE(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
      RETVALUE(RFAILED);
   }

   for(idx=0; idx<elementCnt; idx++)
   {
      RIC_ALLOC(e2SetupRsp->protocolIEs.list.array[idx], \
            sizeof(E2setupResponseIEs_t)); 
      if(e2SetupRsp->protocolIEs.list.array[idx] == NULLP)
      {  
         RIC_FREE(e2SetupRsp->protocolIEs.list.array,\
                   e2SetupRsp->protocolIEs.list.size);
         RIC_FREE(e2apMsg->choice.successfulOutcome, \
               sizeof(SuccessfulOutcomeE2_t));
         RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
         RETVALUE(RFAILED);
      }    
   }
   /* Global RIC ID */
   idx = 0;
   e2SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_GlobalRIC_ID;
   e2SetupRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   e2SetupRsp->protocolIEs.list.array[idx]->value.present = \
                                     E2setupResponseIEs__value_PR_GlobalRIC_ID;

   BuildGlobalRicId(&(e2SetupRsp->protocolIEs.list.array[idx]->value.choice.GlobalRIC_ID));

   xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);

   /* Check encode results */
   if(encRetVal.encoded == ENCODE_FAIL)
   {
	   DU_LOG("\nE2AP : Could not encode E2SetupResponse structure (at %s)\n",\
			   encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
	   RETVALUE(RFAILED);   
   } 
   else 
   {
	   DU_LOG("\nE2AP : Created APER encoded buffer for E2SetupResponse\n");
	   for(int i=0; i< encBufSize; i++)
	   {
		   printf("%x",encBuf[i]);
	   } 
   }


   if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL) != ROK)
   {
      DU_LOG("\nF1AP : Sending E2 Setup Response failed");      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Builds Ric Request Id
 *
 * @details
 *
 *    Function : BuildRicRequestId
 *
 *    Functionality: Building the Ric Request Id
 *
 * @params[in] RICrequestID_t *ricReqId
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildRicRequestId(RICrequestID_t *ricReqId)
{

   if(ricReqId != NULLP)
   {
      ricReqId->ricRequestorID = 1;
      ricReqId->ricInstanceID  = 1;
   }
   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Fills Ric Action To be Setup Item
 *
 * @details
 *
 *    Function : fillSetupItems
 *
 *    Functionality: Filling ricAction Id, RicActionType
 *
 * @params[in] RICaction_ToBeSetup_Item_t *setupItems
 * @return pointer of type RICaction_ToBeSetup_Item_t
 *
 * ****************************************************************/

RICaction_ToBeSetup_Item_t* fillSetupItems(RICaction_ToBeSetup_Item_t *setupItems)
{
   if(setupItems != NULLP)
   {
      setupItems->ricActionID = 0;
      setupItems->ricActionType = RICactionType_report;
   }

   RETVALUE(setupItems);
}

/*******************************************************************
 *
 * @brief Fills RIC Subscription Details Item List
 *
 * @details
 *
 *    Function : fillSubsDetails
 *
 *    Functionality: Fill the RIC Subscription Details Items List
 *
 * @params[in] RICaction_ToBeSetup_ItemIEs_t *items
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 fillSubsDetails(RICaction_ToBeSetup_ItemIEs_t *items)
{
   if(items != NULLP)
   {
      items->id = ProtocolIE_IDE2_id_RICaction_ToBeSetup_Item;
      items->criticality   =  CriticalityE2_ignore;
      items->value.present =  RICaction_ToBeSetup_ItemIEs__value_PR_RICaction_ToBeSetup_Item;
      fillSetupItems(&(items->value.choice.RICaction_ToBeSetup_Item));
   }
   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief builds RIC Subscription Details
 *
 * @details
 *
 *    Function : BuildsRicSubsDetails
 *
 *    Functionality: Builds the RIC Subscription Details
 *
 * @params[in] RICsubscriptionDetails_t *subsDetails
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

S16 BuildRicSubsDetails(RICsubscriptionDetails_t *subsDetails)
{

   U8 elementCnt;

   if(subsDetails != NULLP)
   {
      /* Plmn is not called */
      plmnBuildRic(cuCfgParams.plmn, &subsDetails->ricEventTriggerDefinition);
      elementCnt = 1;
      subsDetails->ricAction_ToBeSetup_List.list.count = elementCnt;
      subsDetails->ricAction_ToBeSetup_List.list.size = \
                      elementCnt * sizeof(RICaction_ToBeSetup_ItemIEs_t);
      RIC_ALLOC(subsDetails->ricAction_ToBeSetup_List.list.array, \
                subsDetails->ricAction_ToBeSetup_List.list.size);
      if(subsDetails->ricAction_ToBeSetup_List.list.array  == NULLP)
      {
         DU_LOG("\nE2AP : Memory allocation for RICactionToBeSetup Items failed");
         RETVALUE(RFAILED);
      } 
      RIC_ALLOC(subsDetails->ricAction_ToBeSetup_List.list.array[0],\
                   sizeof(RICaction_ToBeSetup_ItemIEs_t));
      fillSubsDetails(subsDetails->ricAction_ToBeSetup_List.list.array[0]);
   }
   RETVALUE(ROK);
}

/*******************************************************************
 *
 * @brief Builds and Send the RicSubscriptionReq
 *
 * @details
 *
 *    Function : BuildAndSendRicSubscriptionReq
 *
 * Functionality:Fills the RicSubscriptionReq
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

S16 BuildAndSendRicSubscriptionReq()
{

   E2AP_PDU_t   *e2apRicMsg = NULL;
   RICsubscriptionRequest_t   *ricSubscriptionReq;
   U8   elementCnt;
   U8   idx;
   U8   ieId;
   S16  ret; 
   asn_enc_rval_t             encRetVal;        /* Encoder return value */
   ricSubsStatus = TRUE;

   DU_LOG("\nE2AP : Building RIC Subscription Request\n");

   RIC_ALLOC(e2apRicMsg, sizeof(E2AP_PDU_t));
   if(e2apRicMsg == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2AP-PDU failed");
      RETVALUE(RFAILED);
   }

   e2apRicMsg->present = E2AP_PDU_PR_initiatingMessage;
   RIC_ALLOC(e2apRicMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
   if(e2apRicMsg->choice.initiatingMessage == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for E2AP-PDU failed");
      RIC_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));
      RETVALUE(RFAILED);
   }
   e2apRicMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICsubscription;
   e2apRicMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
   e2apRicMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICsubscriptionRequest;
   
   RIC_ALLOC(ricSubscriptionReq, sizeof(RICsubscriptionRequest_t));
   ricSubscriptionReq = &e2apRicMsg->choice.initiatingMessage->value.choice.RICsubscriptionRequest;
   
   elementCnt = 3;
   ricSubscriptionReq->protocolIEs.list.count = elementCnt;
   ricSubscriptionReq->protocolIEs.list.size  = elementCnt * sizeof(RICsubscriptionRequest_IEs_t);

   /* Initialize the subscription members */
   RIC_ALLOC(ricSubscriptionReq->protocolIEs.list.array, \
              ricSubscriptionReq->protocolIEs.list.size);
   if(ricSubscriptionReq->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nE2AP : Memory allocation for RICSubscriptionRequestIEs failed");
      RIC_FREE(e2apRicMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      RIC_FREE(e2apRicMsg, (Size)sizeof(E2AP_PDU_t));
      RETVALUE(RFAILED);
   }
   
   for(idx=0; idx<elementCnt; idx++)
   {
      RIC_ALLOC(ricSubscriptionReq->protocolIEs.list.array[idx],\
            sizeof(RICsubscriptionRequest_IEs_t));
      if(ricSubscriptionReq->protocolIEs.list.array[idx] == NULLP)
      {
         for(ieId=0; ieId<idx; ieId++)
         {
            RIC_FREE(ricSubscriptionReq->protocolIEs.list.array[ieId],\
                  sizeof(RICsubscriptionRequest_IEs_t));
         }
         RIC_FREE(ricSubscriptionReq->protocolIEs.list.array,\
                  ricSubscriptionReq->protocolIEs.list.size);
         RIC_FREE(e2apRicMsg->choice.initiatingMessage, \
               sizeof(InitiatingMessageE2_t));
         RIC_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));
         RETVALUE(RFAILED);
      }
   }

   /* Filling RIC Request Id */
   idx = 0;
   ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICrequestID;
   ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                    RICsubscriptionRequest_IEs__value_PR_RICrequestID;
 
   BuildRicRequestId(&ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RICrequestID);


   /* Filling RAN Function Id */
   idx++;
   ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionID;
   ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                    RICsubscriptionRequest_IEs__value_PR_RANfunctionID;
   ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RANfunctionID = 1;


   /* Filling RIC Subscription Details */
   idx++;
   ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICsubscriptionDetails;
   ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                    RICsubscriptionRequest_IEs__value_PR_RICsubscriptionDetails;

   BuildRicSubsDetails(&(ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails));


   /* Prints the Msg formed */
   xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apRicMsg);

   cmMemset((U8 *)encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apRicMsg, PrepFinalEncBuf,\
               encBuf);
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG("\nE2AP : Could not encode RicSubscriptionRequest structure (at %s)\n",\
      encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      RETVALUE(RFAILED);
   }
   else
   {
      DU_LOG("\nE2AP : Created APER encoded buffer for RicSubscriptionRequest\n");
      for(int i=0; i< encBufSize; i++)
      {
          printf("%x",encBuf[i]);
      } 
   }


   /* Sending msg */
   if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL) != ROK)
   {
      DU_LOG("\nE2AP : Sending RIC subscription Request failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

 
/*******************************************************************
*
* @brief Handles received E2AP message and sends back response  
*
* @details
*
*    Function : E2APMsgHdlr
*
*    Functionality:
*         - Decodes received E2AP control message
*         - Prepares response message, encodes and sends to SCTP
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void E2APMsgHdlr(Buffer *mBuf)
{
   int i;
   char *recvBuf;
   MsgLen copyCnt;
   MsgLen recvBufLen;
   E2AP_PDU_t *e2apMsg;
   asn_dec_rval_t rval; /* Decoder return value */
   E2AP_PDU_t e2apasnmsg ;
 
   DU_LOG("\nE2AP : Received E2AP message buffer");
   SPrntMsg(mBuf, 0,0);
 
   /* Copy mBuf into char array to decode it */
   SFndLenMsg(mBuf, &recvBufLen);
   if(SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)&recvBuf, (Size)recvBufLen) != ROK)
   {
      DU_LOG("\nE2AP : Memory allocation failed");
      return;
   }
   if(SCpyMsgFix(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
   {
      DU_LOG("\nE2AP : Failed while copying %d", copyCnt);
      return;
   }

   printf("\nE2AP : Received flat buffer to be decoded : ");
   for(i=0; i< recvBufLen; i++)
   {
        printf("%x",recvBuf[i]);
   }

   /* Decoding flat buffer into F1AP messsage */
   e2apMsg = &e2apasnmsg;
   memset(e2apMsg, 0, sizeof(E2AP_PDU_t));

   rval = aper_decode(0, &asn_DEF_E2AP_PDU, (void **)&e2apMsg, recvBuf, recvBufLen, 0, 0);
   SPutSBuf(DFLT_REGION, DFLT_POOL, (Data *)recvBuf, (Size)recvBufLen);
   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nE2AP : ASN decode failed");
      return;
   }
   printf("\n");
   xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

   switch(e2apMsg->present)
   {
      case E2AP_PDU_PR_initiatingMessage:
      {
         switch(e2apMsg->choice.initiatingMessage->value.present)
         {
            case InitiatingMessageE2__value_PR_E2setupRequest:
            {
               DU_LOG("\nE2AP : E2 setup request received");
               BuildAndSendE2SetupRsp();
	       break;
            }
            case InitiatingMessageE2__value_PR_RICindication:
            {
               DU_LOG("\nE2AP : RIC Indication Acknowledged");
	       break;
            }
            default:
            {
               DU_LOG("\nE2AP : Invalid type of intiating message [%d]",e2apMsg->choice.initiatingMessage->value.present);
               return;
            }
         }/* End of switch(initiatingMessage) */
         break;
      }
      case E2AP_PDU_PR_successfulOutcome: 
      {
         switch(e2apMsg->choice.successfulOutcome->value.present)
         {
            case SuccessfulOutcomeE2__value_PR_RICsubscriptionResponse:  
            {
               DU_LOG("\nE2AP : RICsubscriptionResponse Msg Acknowledged");
               break;
            }
            default:
            {
               DU_LOG("\nE2AP : Invalid type of successfulOutcome message [%d]",e2apMsg->choice.successfulOutcome->value.present);
               return;
            }
            break;
         }
         break; 
      }
      default:
      {
         DU_LOG("\nE2AP : Invalid type message type ");
         return;
      }
 
    }/* End of switch(e2apMsg->present) */

    if(!ricSubsStatus)
      BuildAndSendRicSubscriptionReq(); 
       
} /* End of E2APMsgHdlr */


/**********************************************************************
  End of file
 **********************************************************************/