# Install the splrg package from our repository
class splrg::install {
   package { 'splrg':
   ensure => latest,
  }
}

