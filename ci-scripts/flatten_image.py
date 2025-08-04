"""
 Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 contributor license agreements.  See the NOTICE file distributed with
 this work for additional information regarding copyright ownership.
 The OpenAirInterface Software Alliance licenses this file to You under
 the OAI Public License, Version 1.1  (the "License"); you may not use this file
 except in compliance with the License.
 You may obtain a copy of the License at

   http://www.openairinterface.org/?page_id=698

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
-------------------------------------------------------------------------------
 For more information about the OpenAirInterface (OAI) Software Alliance:
   contact@openairinterface.org
"""

import argparse
import re
import subprocess
import sys

def main() -> None:
    args = _parse_args()
    status = perform_flattening(args.tag, args.git_commit)
    sys.exit(status)

def _parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description='Flattening Image')

    parser.add_argument(
        '--tag', '-t',
        action='store',
        required=True,
        help='Image Tag in image-name:image tag format',
    )
    parser.add_argument(
    '--git-commit',
    required=False,
    help='Git commit hash to be embedded in the image label',
    )
    return parser.parse_args()

def perform_flattening(tag, git_commit=None):
    # First detect which docker/podman command to use
    cli = ''
    image_prefix = ''
    cmd = 'which podman || true'
    podman_check = subprocess.check_output(cmd, shell=True, universal_newlines=True)
    if re.search('podman', podman_check.strip()):
        cli = 'sudo podman'
        image_prefix = 'localhost/'
        # since HEALTHCHECK is not supported by podman import
        # we don't flatten
        return 0
    if cli == '':
        cmd = 'which docker || true'
        docker_check = subprocess.check_output(cmd, shell=True, universal_newlines=True)
        if re.search('docker', docker_check.strip()):
            cli = 'docker'
            image_prefix = ''
    if cli == '':
        print ('No docker / podman installed: quitting')
        return -1
    
    # Set container name based on tag
    if 'arm64' in tag:
        container_name = 'test-flatten-arm64'
    else:
        container_name = 'test-flatten'
    print (f'Flattening {tag}')
    # Creating a container
    cmd = f'{cli} run --name {container_name} --entrypoint /bin/true -d {tag}'

    print (cmd)
    subprocess.check_output(cmd, shell=True, universal_newlines=True)

    # Export / Import trick
    cmd = cli + ' export ' + container_name + ' | ' + cli + ' import '
    # Bizarro syntax issue with podman
    if cli == 'docker':
      cmd += ' --change "ENV PATH /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" '
    else:
      cmd += ' --change "ENV PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" '
    cmd += ' --change "WORKDIR /openair-amf" '
    cmd += ' --change "EXPOSE 80/tcp" '
    cmd += ' --change "EXPOSE 9090/tcp" '
    if cli == 'docker':
        cmd += ' --change "EXPOSE 38412/sctp" '
    cmd += ' --change "HEALTHCHECK --interval=10s --timeout=15s --retries=6 CMD /openair-amf/bin/healthcheck.sh" '
    cmd += ' --change "CMD [\\"/openair-amf/bin/oai_amf\\", \\"-c\\", \\"/openair-amf/etc/config.yaml\\", \\"-o\\"]" '
    
    cmd += ' --change "LABEL org.opencontainers.image.authors=\\"OpenAirInterface <contact@openairinterface.org>\\"" '
    cmd += ' --change "LABEL org.opencontainers.image.vendor=\\"OpenAirInterface Software Alliance\\"" '
    cmd += ' --change "LABEL org.opencontainers.image.licenses=\\"OAI Public License, Version 1.1\\"" '
    cmd += ' --change "LABEL org.opencontainers.image.title=\\"OAI AMF\\"" '
    cmd += ' --change "LABEL org.opencontainers.image.description=\\"OpenAirInterface Access and Mobility Management Function\\"" '
    if git_commit:
        cmd += f' --change "LABEL org.opencontainers.image.revision=\\"commit:{git_commit}\\"" '

    
    
    cmd += ' - ' + image_prefix + tag
    print (cmd)
    subprocess.check_output(cmd, shell=True, universal_newlines=True)

    # Remove container
    cmd = f'{cli} rm -f {container_name}'

    print (cmd)
    subprocess.check_output(cmd, shell=True, universal_newlines=True)

    # At this point the original image is a dangling image.
    # CI pipeline will clean up (`image prune --force`)
    return 0

if __name__ == '__main__':
    main()
