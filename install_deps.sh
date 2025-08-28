#!/usr/bin/env bash
set -e

rm -rf deps
mkdir  deps
cd deps

git clone --branch 2.6.x https://github.com/SFML/SFML.git
git clone --branch v1.7.0 https://github.com/texus/TGUI.git
git clone --branch v3.11.2 https://github.com/nlohmann/json.git json