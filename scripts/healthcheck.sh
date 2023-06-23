#!/bin/bash
set -eo pipefail

STATUS=0
# Determine if we are still using conf file or new yaml file
COMMAND_ARGS=$(pgrep oai_amf | xargs ps -fp | grep -c amf.conf || true)

if [ $COMMAND_ARGS -ne 0 ]; then
  # Still using old config file
  AMF_PORT_FOR_NGAP=38412
  AMF_PORT_FOR_N11_HTTP=80
  #Check if entrypoint properly configured the conf file and no parameter is unset (optional)
  NB_UNREPLACED_AT=`cat /openair-amf/etc/*.conf | grep -v contact@openairinterface.org | grep -c @ || true`
else
  # Now using the new yaml file
  AMF_INTERFACE_NAME_FOR_N11=$(yq '.nfs.amf.sbi.interface_name' /openair-amf/etc/config.yaml)
  AMF_PORT_FOR_N11_HTTP=$(yq '.nfs.amf.sbi.port' /openair-amf/etc/config.yaml)
  AMF_INTERFACE_NAME_FOR_NGAP=$(yq '.nfs.amf.n2.interface_name' /openair-amf/etc/config.yaml)
  AMF_PORT_FOR_NGAP=$(yq '.nfs.amf.n2.port' /openair-amf/etc/config.yaml)
  NB_UNREPLACED_AT=0
fi

AMF_IP_NGAP_INTERFACE=$(ifconfig $AMF_INTERFACE_NAME_FOR_NGAP | grep inet | grep -v inet6 | awk {'print $2'})
AMF_IP_N11_INTERFACE=$(ifconfig $AMF_INTERFACE_NAME_FOR_N11 | grep inet | grep -v inet6 | awk {'print $2'})
N2_PORT_STATUS=$(netstat -Snpl | grep -o "$AMF_IP_NGAP_INTERFACE:$AMF_PORT_FOR_NGAP")
N11_PORT_STATUS=$(netstat -tnpl | grep -o "$AMF_IP_N11_INTERFACE:$AMF_PORT_FOR_N11_HTTP")

if [ $NB_UNREPLACED_AT -ne 0 ]; then
	STATUS=1
	echo "Healthcheck error: configuration file is not configured properly"
fi

if [[ -z $N2_PORT_STATUS ]]; then
	STATUS=1
	echo "Healthcheck error: N2 SCTP port $AMF_PORT_FOR_NGAP is not listening"
fi

if [[ -z $N11_PORT_STATUS ]]; then
	STATUS=1
	echo "Healthcheck error: N11/SBI TCP/HTTP port $AMF_PORT_FOR_N11_HTTP is not listening"
fi

exit $STATUS
