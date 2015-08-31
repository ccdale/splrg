# Puppet Configuration for splrgd

## hieradata/common.yaml
 1. add the ````splrg```` class to the list of classes
 2. add the following
````
 sudo::configs:
     ....
     'bgch_service':
        'content' : "Defaults:bgch_service !requiretty\nbgch_service ALL=(root) NOPASSWD: /usr/lib/bgch-puppet/puppet-apply.sh"
```` 
## splrg module
copy the module files from splrg/puppet/module/ to your puppet-code/modules/ directory
