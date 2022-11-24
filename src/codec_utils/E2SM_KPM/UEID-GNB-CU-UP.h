/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-COMMON-IEs"
 * 	found in "O-RAN.WG3.E2SM-v02.01.asn1"
 * 	`asn1c -pdu=auto -fcompound-names -findirect-choice -gen-PER -gen-OER -no-gen-example`
 */

#ifndef	_UEID_GNB_CU_UP_H_
#define	_UEID_GNB_CU_UP_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GNB-CU-CP-UE-E1AP-ID.h"
#include "RANUEID.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UEID-GNB-CU-UP */
typedef struct UEID_GNB_CU_UP {
	GNB_CU_CP_UE_E1AP_ID_t	 gNB_CU_CP_UE_E1AP_ID;
	RANUEID_t	*ran_UEID;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UEID_GNB_CU_UP_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UEID_GNB_CU_UP;
extern asn_SEQUENCE_specifics_t asn_SPC_UEID_GNB_CU_UP_specs_1;
extern asn_TYPE_member_t asn_MBR_UEID_GNB_CU_UP_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _UEID_GNB_CU_UP_H_ */
#include <asn_internal.h>
