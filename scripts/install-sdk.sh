#!/bin/bash

LOCAL_SDK_VERSION=${TABLEAU_SDK_VERSION:-10-2-0}

target=$1

# Install the SDK for linux
if [[ $TRAVIS_OS_NAME == 'linux' ]]; then
  if [[ -z "$target" ]]; then
    wget --directory-prefix=$HOME/tableau-c-sdk https://downloads.tableau.com/tssoftware/Tableau-SDK-Linux-64Bit-${LOCAL_SDK_VERSION}.deb
    sudo dpkg -i $HOME/tableau-c-sdk/Tableau-SDK-Linux-64Bit-${LOCAL_SDK_VERSION}.deb
  else
    wget -O $target.tar.gz https://downloads.tableau.com/tssoftware/Tableau-SDK-Linux-64Bit-${LOCAL_SDK_VERSION}.tar.gz
    mkdir tableausdk && tar zxf tableausdk.tar.gz -C tableausdk --strip-components 1
    rm tableausdk.tar.gz
  fi
fi

# Install the SDK for OSX.
if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  wget --directory-prefix=$HOME/tableau-c-sdk https://downloads.tableau.com/tssoftware/Tableau-SDK-${LOCAL_SDK_VERSION}.dmg
  sudo hdiutil attach $HOME/tableau-c-sdk/Tableau-SDK-${LOCAL_SDK_VERSION}.dmg
  sudo ditto /Volumes/Tableau\ SDK/Frameworks/ /Library/Frameworks/
fi
