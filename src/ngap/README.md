## How to generate libngap files 
- Step 1: Install ASN1C from this repo: https://github.com/mouse07410/asn1c
- Step 2: Extract ASN1-related information from the 3GPP TS 38.413 (just copy the message and information element (with ASN.1) from TS 38.413 and put in e.g., ts38.413.asn1)
- Step 3: Generate NGAP source files with the following commands:

```
$ cd oai-cn5g-amf/src/ngap
$ export ASN1C_PREFIX=Ngap_
$ asn1c ASN1C_PREFIX=Ngap_ -pdu=all -fcompound-names -fno-include-deps -findirect-choice  -no-gen-example -gen-APER -gen-UPER -no-gen-JER -no-gen-BER  -D libngap ./asn_r16/*.asn1

```
