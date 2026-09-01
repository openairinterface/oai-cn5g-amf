#!/bin/bash
# SPDX-License-Identifier: MIT
set -eo pipefail

STATUS=0
AMF_INTERFACE_NAME_FOR_N11=$(yq '.nfs.amf.sbi.interface_name' /openair-amf/etc/config.yaml)
AMF_PORT_FOR_N11_HTTP=$(yq '.nfs.amf.sbi.port' /openair-amf/etc/config.yaml)
AMF_INTERFACE_NAME_FOR_NGAP=$(yq '.nfs.amf.n2.interface_name' /openair-amf/etc/config.yaml)
AMF_PORT_FOR_NGAP=$(yq '.nfs.amf.n2.port' /openair-amf/etc/config.yaml)

AMF_IP_NGAP_INTERFACE=$(ifconfig $AMF_INTERFACE_NAME_FOR_NGAP | grep inet | grep -v inet6 | awk {'print $2'})
AMF_IP_N11_INTERFACE=$(ifconfig $AMF_INTERFACE_NAME_FOR_N11 | grep inet | grep -v inet6 | awk {'print $2'})
N2_PORT_STATUS=$(netstat -Snpl | grep -o "$AMF_IP_NGAP_INTERFACE:$AMF_PORT_FOR_NGAP")
N11_PORT_STATUS=$(netstat -tnpl | grep -o "$AMF_IP_N11_INTERFACE:$AMF_PORT_FOR_N11_HTTP")

if [[ -z $N2_PORT_STATUS ]]; then
	STATUS=1
	echo "Healthcheck error: N2 SCTP port $AMF_PORT_FOR_NGAP is not listening"
fi

if [[ -z $N11_PORT_STATUS ]]; then
	STATUS=1
	echo "Healthcheck error: N11/SBI TCP/HTTP port $AMF_PORT_FOR_N11_HTTP is not listening"
fi

exit $STATUS
