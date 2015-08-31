# Configure the splrg package
class splrg::configure() {
  $splrgusername=hiera('splrg::username')
  $splrggroupname=hiera('splrg::groupname')
  $splrgpuppetlog=hiera('splrg::puppetlog')
  file { '/etc/splrgd.conf':
    mode    => '0644',
    content => template('splrg/splrgd.conf.erb'),
  }
}
