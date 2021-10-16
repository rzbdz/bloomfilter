#!/bin/bash
main() {
  set -o errexit

    if [ $1 == "-y" ] 
    then 
        install
    else
        echo "PACKAGES WILL BE INSTALLED. THIS MAY BREAK YOUR EXISTING TOOLCHAIN."
        echo "YOU ACCEPT ALL RESPONSIBILITY BY PROCEEDING."
        read -p "Proceed? [Y/n] : " yn
    
        case $yn in
            Y|y) install;;
            *) ;;
        esac
    fi

    echo "Script complete."
}


install() {
  set -x
  UNAME=$(uname | tr "[:lower:]" "[:upper:]" )

  case $UNAME in
    DARWIN) install_mac ;;

    LINUX)
      version=$(cat /etc/os-release | grep VERSION_ID | cut -d '"' -f 2)
      case $version in
        18.04) install_linux ;;
        20.04) install_linux ;;
        *) give_up ;;
      esac
      ;;

    *) give_up ;;
  esac
}


give_up() {
  set +x
  echo "Installation Failed, Unsupported distribution '$UNAME'"
  exit 1
}

install_linux() {
  # Update apt-get.
  apt-get -y update
  # Install packages.
  apt-get -y install \
      build-essential \
      clang-format-8 \
      cmake \
      doxygen \
      git \
      g++-7 \
      valgrind 
}

main "$@"
