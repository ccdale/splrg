# splrg Module

class splrg{

  anchor { 'splrg::begin':
    notify  => Class['splrg::install'],
  }

  class { 'splrg::install':
    require => Anchor['splrg::begin'],
    notify  => Class['splrg::configure'],
  }

  class { 'splrg::configure':
    require => Class['splrg::install'],
    notify  => Class['splrg::service'],
  }

  class { 'splrg::service':
    require => Class['splrg::configure'],
    notify  => Anchor['splrg::end'],
  }
  anchor { 'splrg::end': }

}
