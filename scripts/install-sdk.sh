#!/bin/bash

LOCAL_SDK_VERSION=${TABLEAU_SDK_VERSION:-10-2-0}

target=$1

# Install the SDK for linux
if [[ $TRAVIS_OS_NAME == 'linux' ]]; then
  if [[ -z "$target" ]]; then
    wget --directory-prefix=$HOME/tableau-c-sdk https://downloads.tableau.com/tssoftware/Tableau-SDK-Linux-64Bit-${LOCAL_SDK_VERSION}.deb
    sudo dpkg -i $HOME/tableau-c-sdk/Tableau-SDK-Linux-64Bit-${LOCAL_SDK_VERSION}.deb
  else
    if [[ ! -d "lib/${target}" && ! -e "$target.tar.gz" ]]; then
      wget -O $target.tar.gz https://downloads.tableau.com/tssoftware/Tableau-SDK-Linux-64Bit-${LOCAL_SDK_VERSION}.tar.gz
    fi
    [ -d lib ] || mkdir lib
    if [[ ! -d "$target" ]]; then
      mkdir -p "$target" && tar zxf $target.tar.gz -C "$target" --strip-components 1
      rm $target.tar.gz
      mv $target lib
    fi
  fi
fi

# Install the SDK for OSX.
if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  wget --directory-prefix=$HOME/tableau-c-sdk https://downloads.tableau.com/tssoftware/Tableau-SDK-${LOCAL_SDK_VERSION}.dmg
  sudo hdiutil attach $HOME/tableau-c-sdk/Tableau-SDK-${LOCAL_SDK_VERSION}.dmg
  sudo ditto /Volumes/Tableau\ SDK/Frameworks/ /Library/Frameworks/
fi
