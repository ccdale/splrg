# Start the splrg service
class splrg::service() {
  service { 'splrg':
    name       => 'splrgd',
    ensure     => 'running',
    enable     => true,
    hasrestart => true,
  }
}

