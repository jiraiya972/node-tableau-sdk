#!/bin/bash

LOCAL_SDK_VERSION=${TABLEAU_SDK_VERSION:-2018-2-0}

target=$1

# Install the SDK for linux
if [[ $TRAVIS_OS_NAME == 'linux' ]]; then

  if [[ -z "$target" ]]; then
      wget --directory-prefix=$HOME/tableau-c-sdk https://downloads.tableau.com/tssoftware/extractapi-linux-x86_64-${LOCAL_SDK_VERSION}.deb
      sudo dpkg -i $HOME/tableau-c-sdk/extractapi-linux-x86_64-${LOCAL_SDK_VERSION}.deb
  else
    if [[ ! -d "lib/${target}" && ! -e "$target.tar.gz" ]]; then
      wget -O $target.tar.gz https://downloads.tableau.com/tssoftware/extractapi-linux-x86_64-${LOCAL_SDK_VERSION}.tar.gz
      ls -1 $target.tar.gz
      echo "Downloaded Tableau SDK"
    fi
    [ -d lib ] || mkdir lib
    if [[ ! -d "$target" ]]; then
      ls -1 $target.tar.gz
      mkdir -p "$target" && tar zxf $target.tar.gz -C "$target" --strip-components 1
      rm $target.tar.gz
      cp -r $target lib
    fi
  fi
fi

# Install the SDK for OSX.
if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  wget --directory-prefix=$HOME/tableau-c-sdk https://downloads.tableau.com/tssoftware/extractapi-${LOCAL_SDK_VERSION}.dmg
  sudo hdiutil attach $HOME/tableau-c-sdk/extractapi-${LOCAL_SDK_VERSION}.dmg
  sudo ditto /Volumes/Tableau\ Hyper\ Extract\ API/Frameworks/ /Library/Frameworks/
fi
