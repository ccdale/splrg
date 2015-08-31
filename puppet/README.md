# Puppet Configuration for splrgd

## hieradata/common.yaml
 1. add the ````splrg```` package to the list of packages
 2. add the following
 ````
 sudo::configs:
     ....
     'bgch_service':
        'content' : "Defaults:bgch_service !requiretty\nbgch_service ALL=(root) NOPASSWD: /usr/lib/bgch-puppet/puppet-apply.sh"
 ````

