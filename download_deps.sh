#!/usr/bin/env bash
set -e

rm -rf deps
mkdir  deps
cd deps

git clone --branch 2.6.x https://github.com/SFML/SFML.git
git clone --branch v1.7.0 https://github.com/texus/TGUI.git
git clone --branch v3.11.2 https://github.com/nlohmann/json.git json

#!/bin/bash

# Установка всех зависимостей для сборки проекта CMake
echo "Установка зависимостей для сборки проекта..."

# Определение дистрибутива и пакетного менеджера
if command -v apt &> /dev/null; then
    PACKAGE_MANAGER="apt"
    INSTALL_CMD="sudo apt install -y"
    UPDATE_CMD="sudo apt update"
elif command -v dnf &> /dev/null; then
    PACKAGE_MANAGER="dnf"
    INSTALL_CMD="sudo dnf install -y"
    UPDATE_CMD="sudo dnf check-update"
elif command -v yum &> /dev/null; then
    PACKAGE_MANAGER="yum"
    INSTALL_CMD="sudo yum install -y"
    UPDATE_CMD="sudo yum check-update"
elif command -v pacman &> /dev/null; then
    PACKAGE_MANAGER="pacman"
    INSTALL_CMD="sudo pacman -S --noconfirm"
    UPDATE_CMD="sudo pacman -Sy"
elif command -v zypper &> /dev/null; then
    PACKAGE_MANAGER="zypper"
    INSTALL_CMD="sudo zypper install -y"
    UPDATE_CMD="sudo zypper refresh"
else
    echo "Не удалось определить пакетный менеджер!"
    exit 1
fi

echo "Обнаружен пакетный менеджер: $PACKAGE_MANAGER"

# Обновление репозиториев
echo "Обновление репозиториев..."
$UPDATE_CMD

# Установка базовых инструментов разработки
echo "Установка базовых инструментов разработки..."
case $PACKAGE_MANAGER in
    "apt")
        $INSTALL_CMD build-essential cmake pkg-config
        ;;
    "dnf"|"yum")
        $INSTALL_CMD gcc-c++ make cmake pkgconfig
        ;;
    "pacman")
        $INSTALL_CMD base-devel cmake pkgconf
        ;;
    "zypper")
        $INSTALL_CMD patterns-devel-base-devel_basis cmake pkg-config
        ;;
esac

# Установка X11 библиотек
echo "Установка X11 библиотек..."
case $PACKAGE_MANAGER in
    "apt")
        $INSTALL_CMD libx11-dev libxext-dev libxfixes-dev libxrender-dev \
                   libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev \
                   libxcomposite-dev libxdamage-dev
        ;;
    "dnf"|"yum")
        $INSTALL_CMD libX11-devel libXext-devel libXfixes-devel libXrender-devel \
                   libXcursor-devel libXrandr-devel libXinerama-devel libXi-devel \
                   libXcomposite-devel libXdamage-devel
        ;;
    "pacman")
        $INSTALL_CMD libx11 libxext libxfixes libxrender \
                   libxcursor libxrandr libxinerama libxi
        ;;
    "zypper")
        $INSTALL_CMD libX11-devel libXext-devel libXfixes-devel libXrender-devel \
                   libXcursor-devel libXrandr-devel libXinerama-devel libXi-devel
        ;;
esac

# Установка OpenGL библиотек
echo "Установка OpenGL библиотек..."
case $PACKAGE_MANAGER in
    "apt")
        $INSTALL_CMD libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
        ;;
    "dnf"|"yum")
        $INSTALL_CMD mesa-libGL-devel mesa-libGLU-devel freeglut-devel
        ;;
    "pacman")
        $INSTALL_CMD mesa freeglut
        ;;
    "zypper")
        $INSTALL_CMD Mesa-libGL-devel Mesa-libGLU-devel freeglut-devel
        ;;
esac

# Установка FreeType
echo "Установка FreeType..."
case $PACKAGE_MANAGER in
    "apt")
        $INSTALL_CMD libfreetype6-dev
        ;;
    "dnf"|"yum")
        $INSTALL_CMD freetype-devel
        ;;
    "pacman")
        $INSTALL_CMD freetype2
        ;;
    "zypper")
        $INSTALL_CMD freetype2-devel
        ;;
esac

# Дополнительные библиотеки, которые часто требуются
echo "Установка дополнительных библиотек..."
case $PACKAGE_MANAGER in
    "apt")
        $INSTALL_CMD libssl-dev libpng-dev libjpeg-dev zlib1g-dev
        ;;
    "dnf"|"yum")
        $INSTALL_CMD openssl-devel libpng-devel libjpeg-turbo-devel zlib-devel
        ;;
    "pacman")
        $INSTALL_CMD openssl libpng libjpeg-turbo zlib
        ;;
    "zypper")
        $INSTALL_CMD libopenssl-devel libpng16-devel libjpeg-devel zlib-devel
        ;;
esac
